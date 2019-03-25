#include <file_forensic.h>

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

char* selectYear(char * data_time){
    char *year = malloc(5);
    memcpy(year, &data_time[20], 4);

    return year;
}

char* selectMonth(char * data_time){
    char month[4];
    memcpy(month, &data_time[4], 3);

    char* solution = malloc(3);

    switch(month[0]){
        case 'J': //JANUARY, JUNE AND JULY
            if(month[1] == 'a'){
                solution = "01";
            }
            else if(month[2] == 'n'){
                solution = "06";
            }
            else solution = "07";
            break;
        case 'F': //FEBRUARY
            solution = "02";
            break;
        case 'M': //MARCH AND MAY
            if(month[2] == 'r'){
                solution = "03";
            }
            else solution = "04";
            break;
        case 'A': //APRIL AND AUGUST
            if(month[1] == 'p'){
                solution = "04";
            }
            else solution = "08";
            break;
        case 'S': //SEPTEMBER
            solution = "09";
            break;
        case 'O': //OCTOBER
            solution = "10";
            break;
        case 'N': //NOVEMBER
            solution = "11";
            break;
        case 'D': //DECEMBER
            solution = "12";
            break;
    }
    return solution;
}

char* selectDay(char * data_time){
    char *day = malloc(3);
    memcpy(day, &data_time[8], 2);

    return day;
}

char* selectHour(char * data_time){
    char *hour = malloc(9);
    memcpy(hour, &data_time[11], 8);

    return hour;
}

char* getDate(char* date_time){
    char* info = malloc(20);

    sprintf(info, "%s-%s-%sT%s", selectYear(date_time), selectMonth(date_time), selectDay(date_time), selectHour(date_time));

    return info;
}

char* selectPermissions(mode_t mode){
    char* aux = malloc(10);

    sprintf(aux, (mode & S_IRUSR) ? "r" : "-");
    sprintf(aux, (mode & S_IWUSR) ? "w" : "-");
    sprintf(aux, (mode & S_IXUSR) ? "x" : "-");
    sprintf(aux, (mode & S_IRGRP) ? "r" : "-");
    sprintf(aux, (mode & S_IWGRP) ? "w" : "-");
    sprintf(aux, (mode & S_IXGRP) ? "x" : "-");
    sprintf(aux, (mode & S_IROTH) ? "r" : "-");
    sprintf(aux, (mode & S_IWOTH) ? "w" : "-");
    sprintf(aux, (mode & S_IXOTH) ? "x" : "-");

    return aux;
}

//file analysis functions
char * getFileInfo(char * file_name){ //1,2,3,4,5,6
    int pid;
    char * info;
    int pipe_des[2];
    char * buffer;

    if(pipe(pipe_des) != 0){
      perror("Failed to open pipe descriptors\n");  
    } 

    pid = fork();

    if(pid == 0){
        //child send info
        close(pipe_des[0]);

        if(dup2(pipe_des[1], STDOUT_FILENO) != 0){
            perror("Failed dup2\n");
        }
        
        execlp("file", file_name, NULL);
    }
    else{
        close(pipe_des[1]);
        
        while(read(pipe_des[0], buffer, MAX_BUF) > 0){
            info = realloc(info, strlen(buffer) + strlen(info) + 1);
            strcat(info, buffer);
        }
    }
    
    return info;
}

char * getFileStatus(char* file_name){
    struct stat statbuf;

    lstat(file_name, &statbuf);

    char* info;
    
    char* date_time = malloc(20);

    sprintf(info, ", %ld,", statbuf.st_size);

    sprintf(info, " %s,", selectPermissions(statbuf.st_mode));

    strcpy(date_time, ctime(&statbuf.st_atime));

    sprintf(info, ", %s,", getDate(date_time));

    strcpy(date_time, ctime(&statbuf.st_mtime));

    sprintf(info, ", %s,", getDate(date_time));

    char* result = malloc(strlen(info));

    strcpy(result, info);

    free(info);
    
    return result;
}
