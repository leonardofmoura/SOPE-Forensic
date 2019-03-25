#include <file_forensic.h>

//7-md5sum hash
//8-sha1sum hash       //all of these in hexadecimal
//9-sha256sum hash

char * concatenate(char * old, char * new){   
    char * result = malloc(strlen(old) + strlen(new) + 1);

    strcpy(result, old);

    result += strlen(old);

    strcpy(result, new);

    if(*old){
        free(old);
    }

    if(*new){
        free(new);
    }

    return result;
}

//file analysis functions
char * file(char * file_name){ //1,2,3,4,5,6
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
        
        execlp(file, file_name, NULL);
    }
    else{
        close(pipe_des[1]);
        
        while(read(pipe_des[0], buffer, MAX_BUF) > 0){
            info = concatenate(info, buffer);
        }
    }

    return info;
}

char * getFileStatus(char* file_name){
    struct stat statbuf;

    lstat(file_name, &statbuf);

    char* info;

    //sprintf(info, "%d, %c%c, %s", statbuf.st_size,  , statbuf.)

    return info;
}

