#include <file_forensic.h>
#include <hash_functions.h>
#include <file_logging.h>
#include "signal_handlers.h"
#include <unistd.h>

#define DATE_TIME_SIZE      19
#define PERMISSIONS_SIZE    9
#define COMMA_SPACE_SIZE    2

#define SUCCESS     0
#define FAIL        1


void getParcell(int number, int increment,char* result){
    //char* result = malloc(3);

    if(increment == 2 && number == 60){
        sprintf(result, "00");
        return;// result;
    }

    if(number > 9){
        sprintf(result, "%d", number + increment);
    }
    else sprintf(result, "0%d", number + increment);

    return;//result;
}

void getDate(time_t * date_time,char* date){
    //char info = malloc(20);

    struct tm *dates = gmtime(date_time);

    char* tmp = calloc(10,1);
    getParcell(dates->tm_mon, 1,tmp);
    sprintf(date, "%d-%s-", 
        dates->tm_year + 1900, tmp);
    
    getParcell(dates->tm_mday, 0,tmp);
    strcat(date,tmp);
    strcat(date,"T");
    getParcell(dates->tm_hour, 0,tmp);
    strcat(date,tmp);
    strcat(date,":");
    getParcell(dates->tm_min, 0,tmp);
    strcat(date,tmp);
    strcat(date,":");
    getParcell(dates->tm_sec, 2,tmp);
    strcat(date,tmp);

    free(tmp);

    //return info;
}

void selectPermissions(mode_t mode,char* perm){
    //char* aux = malloc(10);

    sprintf(perm, "%c%c%c%c%c%c%c%c%c", 
        (mode & S_IRUSR) ? 'r' : '-', 
        (mode & S_IWUSR) ? 'w' : '-', 
        (mode & S_IXUSR) ? 'x' : '-', 
        (mode & S_IRGRP) ? 'r' : '-', 
        (mode & S_IWGRP) ? 'w' : '-', 
        (mode & S_IXGRP) ? 'x' : '-', 
        (mode & S_IROTH) ? 'r' : '-', 
        (mode & S_IWOTH) ? 'w' : '-', 
        (mode & S_IXOTH) ? 'x' : '-');

    //return aux;
}

void fixInfo(char * info){
    bool flag = false;
    for(int i = 0; i < strlen(info); i++){
        if(info[i] == ':'){
            info[i] = ',';
        }
        else if(info[i] == ' ') {
            if(flag)
                continue;
            memmove(&info[i], &info[i + 1], strlen(info) - i);
            i++;
            flag = true;
        }
        else if(info[i] == ',' || info[i] == '\n'){
            info[i] = '\0';
            //info = realloc(info, i);
            return;
        }
    }
}

//file analysis functions
void getFileInfo(char * file_name, char* info){ //1,2,3,4,5,6
    int pid;
    // char * info = calloc(1, 1);
    int pipe_des[2];
    char buffer[MAX_BUF];

    if(pipe(pipe_des) != 0){
      perror("Failed to open pipe descriptors\n");  
    } 

    pid = fork();

    if(pid == 0){
        //child send info
        close(pipe_des[0]);

        dup2(pipe_des[1], STDOUT_FILENO);

        execlp("file", "file", file_name, NULL);

        //not necessary to close, for exec call does not allow to read code further
    }
    else{
        close(pipe_des[1]);

        int n;

        while((n = read(pipe_des[0], buffer, MAX_BUF - 1)) > 0){
            strcat(info, buffer);
        }

        close(pipe_des[0]);
    }
    
    fixInfo(info);
    
    // return info;
}

void getFileStatus(char* file_name, char* info){
    struct stat statbuf;

    lstat(file_name, &statbuf);

    char aux[sizeof(statbuf.st_size)/sizeof(off_t) + PERMISSIONS_SIZE + 2*DATE_TIME_SIZE + 4*COMMA_SPACE_SIZE + 1];
    char* tmp = malloc(MAX_BUF);
    selectPermissions(statbuf.st_mode,tmp); 
    sprintf(aux, ",%ld,%s,",statbuf.st_size,tmp); 
    getDate(&statbuf.st_atime,tmp); 
    strcat(aux,tmp);
    strcat(aux,",");
    getDate(&statbuf.st_mtime,tmp);
    strcat(aux,tmp);
    strcat(info, aux);
    free(tmp);
    // return info;
}

void getFileHash(char* file_name, struct Contents* contents, char* result){    
    if(contents->md5_hash) {
        char aux[100];
        md5_sum(file_name, aux);
        strcat(result, ",");
        strcat(result, aux);
    }

    if(contents->sha1_hash) {
        char aux[100];
        sha1_sum(file_name,aux);
        strcat(result, ",");
        strcat(result, aux);
    }

    if(contents->sha256_hash) {
        char aux[200];
        sha256_sum(file_name,aux);
        strcat(result, ",");
        strcat(result, aux);
    }
}

int file_forensic(char* file_name, struct Contents* contents, char* result) {   
    
    verbose_analized(getpid(),file_name);

    getFileInfo(file_name, result);   

    getFileStatus(file_name, result);

    getFileHash(file_name, contents, result);

    if (get_sigint()) {
        exit(2);
    }

    return SUCCESS;
}
