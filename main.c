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
    if(contents->md5_hash) {
        printf("MD5 selected.\n");
    }

    if(contents->sha1_hash) {
        printf("SHA1 selected.\n");
    }

    if(contents->sha256_hash) {
        printf("SHA256 selected.\n");
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
    //display_info(&cont);

    int fd;
    if(cont.outfile != NULL) {
        fd = open(cont.outfile, O_WRONLY | O_CREAT,0644);
        if(fd == -1) {
            perror(cont.outfile);
            close(fd);
            return 4;
        }
        printf("Data saved on file %s\n",cont.outfile);
        dup2(fd,STDOUT_FILENO);
        close(fd);
    }

    if(cont.dir_name != NULL) {
        int return_value = 0;
        if((return_value = recursive_forensic(cont.dir_name,&cont)) !=0) {
            perror(cont.dir_name);
            return return_value;
        }
        return 0;
    }

    if(cont.file_name != NULL) {
        char* result = malloc(MAX_BUF);

        if(file_forensic(cont.file_name, &cont, result) != 0){
            perror("Forensic Error\n");
        }

        printf("%s\n", result);

        free(result);
    }

    return 0;
}