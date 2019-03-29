#include <file_forensic.h>
#include <hash_functions.h>

#define DATE_TIME_SIZE      19
#define PERMISSIONS_SIZE    9
#define COMMA_SPACE_SIZE    2
//7-md5sum hash
//8-sha1sum hash       //all of these in hexadecimal
//9-sha256sum hash

// char * concatenate(char * old, char * new){   
//     char * result = malloc(strlen(old) + strlen(new) + 1);

//     strcpy(result, old);

//     result += strlen(old);

//     strcpy(result, new);

//     free(old);
//     free(new);

//     return result;
// }

char* getParcell(int number, int increment){
    char* result = malloc(3);

    if(increment == 2 && number == 60){
        sprintf(result, "00");
        return result;
    }

    if(number > 9){
        sprintf(result, "%d", number + increment);
    }
    else sprintf(result, "0%d", number + increment);

    return result;
}

char* getDate(time_t * date_time){
    char* info = malloc(20);

    struct tm *dates = gmtime(date_time);

    sprintf(info, "%d-%s-%sT%s:%s:%s", 
        dates->tm_year + 1900, 
        getParcell(dates->tm_mon, 1), 
        getParcell(dates->tm_mday, 0),
        getParcell(dates->tm_hour, 0),
        getParcell(dates->tm_min, 0),
        getParcell(dates->tm_sec, 2));

    return info;
}

char* selectPermissions(mode_t mode){
    char* aux = malloc(10);

    sprintf(aux, "%c%c%c%c%c%c%c%c%c", 
        (mode & S_IRUSR) ? 'r' : '-', 
        (mode & S_IWUSR) ? 'w' : '-', 
        (mode & S_IXUSR) ? 'x' : '-', 
        (mode & S_IRGRP) ? 'r' : '-', 
        (mode & S_IWGRP) ? 'w' : '-', 
        (mode & S_IXGRP) ? 'x' : '-', 
        (mode & S_IROTH) ? 'r' : '-', 
        (mode & S_IWOTH) ? 'w' : '-', 
        (mode & S_IXOTH) ? 'x' : '-');

    return aux;
}

void fixInfo(char * info){
    for(int i = 0; i < strlen(info); i++){
        if(info[i] == ':'){
            info[i] = ',';
        }
        else if(info[i] == ','){
            info[i] = '\0';
            info = realloc(info, i);
        }
    }
}

//file analysis functions
char * getFileInfo(const char * file_name){ //1,2,3,4,5,6
    int pid;
    char * info = calloc(1, 1);
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
    }
    else{
        close(pipe_des[1]);

        int n;

        while((n = read(pipe_des[0], buffer, MAX_BUF - 1)) > 0){
            buffer[n] = '\0';
            info = realloc(info, strlen(buffer) + strlen(info));
            strcat(info, buffer);
        }
    }
    
    fixInfo(info);

    return info;
}

char * getFileStatus(const char* file_name){
    struct stat statbuf;

    lstat(file_name, &statbuf);

    char* info = malloc(sizeof(statbuf.st_size)/sizeof(off_t) + PERMISSIONS_SIZE + 2*DATE_TIME_SIZE + 4*COMMA_SPACE_SIZE + 1);
    
    sprintf(info, "%ld, %s, %s, %s", 
        statbuf.st_size, 
        selectPermissions(statbuf.st_mode), 
        getDate(&statbuf.st_atime), 
        getDate(&statbuf.st_mtime));

    return info;
}

char* getFileHash(const char* file_name, char* hashes[]){
    char* result;
    char* aux;

    for(int i =0; hashes[i] != NULL;i++) {
        
        if(strcmp(hashes[i],"md5") == 0) {
            md5_sum(file_name,md5sum);
            result = realloc(result, strlen(md5sum) + COMMA_SPACE_SIZE + strlen(result));
            strcat(result, ", ");
            strcat(result, md5sum);
        }

        if(strcmp(hashes[i],"sha1")==0) {
            sha1_sum(file_name, sha1sum);
            result = realloc(result, strlen(sha1sum) + COMMA_SPACE_SIZE + strlen(result));
            strcat(result, ", ");
            strcat(result, sha1sum);
        }

        if(strcmp(hashes[i],"sha256")==0) {
            sha256_sum(file_name, sha256sum);
            result = realloc(result, strlen(sha256sum) + COMMA_SPACE_SIZE + strlen(result));
            strcat(result, ", ");
            strcat(result, sha256sum);
        }        
    } 

    free(md5sum);
    free(sha1sum);
    free(sha256sum);

    return result;
}

char* file_forensic(const char* file_name, char* hashes[]) {  
    char *result = getFileInfo(file_name);
    char *aux = getFileStatus(file_name);

    result = realloc(result, strlen(aux) + COMMA_SPACE_SIZE + strlen(result));
    strcat(result, ", ");
    strcat(result, aux);
    
    aux = getFileHash(file_name, hashes);

    if(aux != NULL){
        result = realloc(result, strlen(aux) + COMMA_SPACE_SIZE + strlen(result));
        strcat(result, ", ");
        strcat(result, aux);
    }

    free(aux);
    
    return result;
}
