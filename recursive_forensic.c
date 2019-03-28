#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <recursive_forensic.h>
#include <file_forensic.h>

int recursive_forensic(const char* dir_path, struct Contents* content) {
    DIR* dir_ptr;
    struct dirent *dentry;
    struct stat stat_entry;

    int proc_ongoing =0;
    
    char path[2*MAX_BUFFER];
    getcwd(path,MAX_BUFFER);
    strcat(path,"/");
    strcat(path,dir_path);
    chdir(path);
    if((dir_ptr = opendir(path)) == NULL) {
        perror(dir_path);
        return 1;
    }
    
    while((dentry = readdir(dir_ptr)) != NULL) {
        
        //update path
        strcat(path,"/");
        strcat(path,dentry->d_name);
        
        if (lstat(dentry->d_name,&stat_entry) < 0) {
            perror("Failed lstat() call.");
            return 2;
        }

        if (S_ISREG(stat_entry.st_mode)) {
            pid_t pid = fork();
            if( pid < 0) {
                perror("Fork failed.");
                return 3;
            }

            if( pid == 0) {
                //log action
                if(file_forensic(dentry->d_name,content->hashes) !=0) {
                    perror(content->file_name);
                    return 4;
                }
                printf("FILE ANALYSED: %s\n",dentry->d_name);
                exit(0);
            }

            else {
                
                proc_ongoing++;
            }
        }
        else if(S_ISDIR(stat_entry.st_mode)) {
            if(strncmp(dentry->d_name,".",1) == 0 || strncmp(dentry->d_name,"..",2) == 0) {
                //current directory or parent
                //not doing anything
                continue;
            }

            pid_t pid = fork();
            if(pid < 0) {
                perror("Fork failed.");
                return 5;
            }

            if(pid == 0) {
                recursive_forensic(dentry->d_name,content);
                //log action
                printf("DIRECTORY ANALYSED: %s\n",dentry->d_name);
                exit(0);
            }
            else {
                
                proc_ongoing++;
            }
        }
    }

    closedir(dir_ptr);
    int status, result;
    while(proc_ongoing > 0) {
        result = wait(&status);
        if(result <0) {
            return 6;
        }
        else {
            if(status == 0) {
                proc_ongoing--;
            }
            else {
                return result;
            }
        }
    }

    return 0;
}