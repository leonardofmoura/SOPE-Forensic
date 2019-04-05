#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include "recursive_forensic.h"
#include "file_forensic.h"
#include "signal_handlers.h"

int recursive_forensic(char* dir_path, struct Contents* content) {
    DIR* dir_ptr;
    struct dirent *dentry;
    struct stat stat_entry;
    
    char path[2*MAX_BUFFER]= "";
    
    if(dir_path[0] != '/') {
    getcwd(path,MAX_BUFFER);            //get the path to the directory to analyse
    strcat(path,"/");
    }

    strcat(path,dir_path);

    if((dir_ptr = opendir(dir_path)) == NULL) {
        printf("FAILED TO OPEN DIR ON PATH: '%s'\n",path);
        perror(dir_path);
        return 1;
    }
    
    while((dentry = readdir(dir_ptr)) != NULL) {
        
        sprintf(path,"%s/%s",dir_path,dentry->d_name);
        if (lstat(path,&stat_entry) < 0) {
            perror("Failed lstat() call.");
            return 2;
        }

        if (S_ISREG(stat_entry.st_mode)) {
            
                char result[2*MAX_BUF] = "";
                if(file_forensic(path,content, result) !=0) {
                    printf("FAILED TO ANALYZE FILE: '%s'\n",dentry->d_name);
                    perror(content->file_name);
                    return 4;
                }

                printf("%s\n", result);
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
                if (get_sigint()) {
                    exit(2);
                }
                
                if(recursive_forensic(path,content) != 0) {
                    perror(dentry->d_name);
                    return 6;
                };
                //log action
                exit(0);
            }

            else {
                    int status;
                    while(wait(&status) > 0);
            }
        }
    }

    closedir(dir_ptr);

    return 0;
}