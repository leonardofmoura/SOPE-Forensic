#include <file_forensic.h>
#include <hash_functions.h>

#define DATE_TIME_SIZE      19
#define PERMISSIONS_SIZE    9
#define COMMA_SPACE_SIZE    2

#define SUCCESS     0
#define FAIL        1


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
        else if(info[i] == ' ') {
            memmove(&info[i], &info[i + 1], strlen(info) - i);
            i++;
        }
        else if(info[i] == ',' || info[i] == '\n'){
            info[i] = '\0';
            info = realloc(info, i);
            return;
        }
        else if(info[i] == ' ') {
            memmove(&info[i], &info[i + 1], strlen(info) - i);
            i++;
        }
    }
}

//file analysis functions
void getFileInfo(const char * file_name, char* info){ //1,2,3,4,5,6
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
    }
    else{
        close(pipe_des[1]);

        int n;

        while((n = read(pipe_des[0], buffer, MAX_BUF - 1)) > 0){
            // buffer[n] = '\0';
            // info = realloc(info, n + strlen(info));
            strcat(info, buffer);
        }
    }
    
    fixInfo(info);

    // return info;
}

void getFileStatus(const char* file_name, char* info){
    struct stat statbuf;

    lstat(file_name, &statbuf);

    char aux[sizeof(statbuf.st_size)/sizeof(off_t) + PERMISSIONS_SIZE + 2*DATE_TIME_SIZE + 4*COMMA_SPACE_SIZE + 1];
    sprintf(aux, ",%ld,%s,%s,%s", 
    // char* info = malloc(sizeof(statbuf.st_size)/sizeof(off_t) + PERMISSIONS_SIZE + 2*DATE_TIME_SIZE + 4*COMMA_SPACE_SIZE + 1);

        statbuf.st_size, 
        selectPermissions(statbuf.st_mode), 
        getDate(&statbuf.st_atime), 
        getDate(&statbuf.st_mtime));

    strcat(info, aux);
    // return info;
}

void getFileHash(const char* file_name, struct Contents* contents, char* result){    
    if(contents->md5_hash) {
        strcat(result, ",");
        char aux[100];
        sha1_sum(file_name, aux);
        // result = realloc(result, strlen(aux) + COMMA_SPACE_SIZE + strlen(result));
        strcat(result, ",");
        strcat(result, aux);
    }

    if(contents->sha1_hash) {
        char aux[100];
        sha1_sum(file_name,aux);
        // result = realloc(result, strlen(aux) + COMMA_SPACE_SIZE + strlen(result));
        strcat(result, ",");
        strcat(result, aux);
    }

    if(contents->sha256_hash) {
        char aux[100];
        sha256_sum(file_name,aux);
        // result = realloc(result, strlen(aux) + COMMA_SPACE_SIZE + strlen(result));
        strcat(result, ",");
        strcat(result, aux);
    }
}

int file_forensic(const char* file_name, struct Contents* contents, char* result) {   
    getFileInfo(file_name, result);

    getFileStatus(file_name, result);

    // result = realloc(result, strlen(aux) + COMMA_SPACE_SIZE + strlen(result));
    // strcat(result, ", ");
    // strcat(result, aux);

    getFileHash(file_name, contents, result);

    return SUCCESS;
}
