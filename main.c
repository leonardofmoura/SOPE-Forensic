#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include <input_parser.h>
#include <hash_functions.h>
#include <recursive_forensic.h>

//include de ficheiros de fora
#include "file_forensic.h"

void show_usage() {
    printf("Usage: forensic [-r] [-h [md5[,sha1[,sha256]]] [-o <outfile>] [-v] <file|dir>\n");
}

void display_info(struct Contents * contents) {
    //first check for recursive analysis on directories
    if(contents->dir_name != NULL) {
        printf("Directory name: %s\n",contents->dir_name);
    }   
    else if(contents->file_name != NULL) {
        printf("File name: %s\n",contents->file_name);
    }
    else {
            printf("File name missing.\n");
            show_usage();
            return;
    }

    //check if the hash option has been selected
    if(contents->hashes!= NULL) {
        int i =0;
        while(contents->hashes[i] != NULL) {
            printf("Hash nº%d: %s\n",i+1,contents->hashes[i]);
            i++;
        }
    }

    //check if the outfile option has been selected
    if(contents->outfile != NULL) {
        printf("Output file: %s\n",contents->outfile);
    }

    //check if the execution log option has been selected
    if(strcmp(contents->log_check,"true")== 0) {
        printf("Logging execution events.\n");
    } 
    else {
        printf("Not logging execution events.\n");
    }

    return;
}

int main(int argc, char* argv[]) {

    if(argc == 1) {
        //invalid arguments
        show_usage();
        return 1;
    }
    
    //parse the command given
    struct Contents cont;
    memset(&cont,0,sizeof(cont));
    //cont = malloc(sizeof(struct Contents)); // [file,folder,hashes,outfile,log]   
    if(input_parser(argc,argv,&cont) != 0) {
        printf("Invalid option or wrong number of arguments.\n");
        show_usage();
        return 2;
    }

    //Just display the info collected after parsing;
    display_info(&cont);

    int fd;
    if(cont.outfile != NULL) {
        fd = open(cont.outfile, O_WRONLY | O_CREAT,0644);
        if(fd == -1) {
        printf("What is foin\n");
            perror(cont.outfile);
            close(fd);
            return 4;
        }
        dup2(fd,STDOUT_FILENO);
        close(fd);
    }

    if(cont.dir_name != NULL) {
        int return_value = 0;
        if((return_value = recursive_forensic(cont.dir_name,&cont)) !=0) {
            printf("Kaboom! Abandon Ship!\n");
            perror(cont.dir_name);
            return return_value;
        }
        return 0;
    }

    //Commented only because it was faulty
    //But im waiting for your fix Sofia-chan
    /*
    if(cont.file_name != NULL){
        char *result = getFileInfo(cont.file_name);
        printf("%s, ", result);

        free(result);

        result = getFileStatus(cont.file_name);
        printf("%s", result);

        free(result);
    }*/


    //TO CLEAN UP
    char* md5sum = malloc(MAX_BUFFER);
    char* sha1sum = malloc(MAX_BUFFER);
    char* sha256sum = malloc(MAX_BUFFER);
    
    for(int i =0; cont.hashes[i] != NULL;i++) {
        
        if(strcmp(cont.hashes[i],"md5") == 0) {
            md5_sum(cont.file_name,md5sum);
            printf("MD5 sum: %s\n",md5sum);
        }

        if(strcmp(cont.hashes[i],"sha1")==0) {
            sha1_sum(cont.file_name,sha1sum);
            printf("SHA1 sum: %s\n",sha1sum);
        }

        if(strcmp(cont.hashes[i],"sha256")==0) {
            sha256_sum(cont.file_name,sha256sum);
            printf("SHA256 sum: %s\n",sha256sum);
        }        
    }
    





    //free alocated memory
    {   
        free(md5sum);
        free(sha1sum);
        free(sha256sum);
        
    }
    return 0;
}