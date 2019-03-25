#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include <input_parser.h>
#include <file_logging.h>
//include de ficheiros de fora
#include <hash_functions.h>

//include de ficheiros de fora
#include "file_forensic.h"

void show_usage() {
    printf("Usage: forensic [-r] [-h [md5[,sha1[,sha256]]] [-o <outfile>] [-v] <file|dir>\n");
}

int proc_args(char* opts[],char* dir_name, char* file_name, char* hashes[],char* outfile) {
    //first check for recursive analysis on directories
    if(opts[1] != NULL) {
        strcpy(dir_name,opts[1]);
        printf("Directory name: %s\n",dir_name);
    }   
    else if(opts[0] != NULL) {

        strcpy(file_name,opts[0]);
        printf("File name: %s\n",file_name);
    }
    else {
        printf("File name missing.\n");
        show_usage();
        return -1;
    }

    //check if the hash option has been selected
    if(opts[2] != NULL) {
        
        hashes[0] = strtok(opts[2],","); //break the string into tokens
        int i =1;
        while((hashes[i] = strtok(NULL,",")) != NULL) {
            i++;
        }
        hashes[3] = NULL;
    }

    //check if the outfile option has been selected
    if(opts[3] != NULL) {

        strcpy(outfile,opts[3]);
        printf("Output file: %s\n",outfile);
    }

    //check if the execution log option has been selected
    if(strcmp(opts[4],"true")== 0) {
        init_time();
    } 
    else {
        printf("Not logging execution events.\n");
    }

    return 0;
}

int main(int argc, char* argv[]) {

    if(argc == 1) {
        //invalid arguments
        show_usage();
        return 1;
    }
    
    //parse the command given
    char * opts[5] = {NULL}; // [file,folder,hashes,outfile,log]   
    if(input_parser(argc,argv,opts) != 0) {
        printf("Invalid option or wrong number of arguments.\n");
        show_usage();
        return 2;
    }

    char* dir_name = malloc(MAX_BUFFER);
    char* file_name = malloc(MAX_BUFFER);
    char* hashes[4] = {malloc(MAX_BUFFER)};
    char* outfile = malloc(MAX_BUFFER);

    char* md5sum = malloc(MAX_BUFFER);
    char* sha1sum = malloc(MAX_BUFFER);
    char* sha256sum = malloc(MAX_BUFFER);
    //process parsed and check for missing info
    if (proc_args(opts,dir_name,file_name,hashes,outfile) !=0) {
        show_usage();
        return 3;
    };

    if(file_name != NULL){
        char *result = getFileInfo(file_name);
        verbose_analized(getpid(),file_name);
        printf("%s, ", result);

        free(result);

        result = getFileStatus(file_name);
        printf("%s", result);

        free(result);
    }

    for(int i =0; hashes[i] != NULL;i++) {
        
        if(strcmp(hashes[i],"md5") == 0) {
            md5sum = strtok(md5_sum(file_name)," ");
            printf("MD5 sum: %s\n",md5sum);
        }

        if(strcmp(hashes[i],"sha1")==0) {
            sha1sum = strtok(sha1_sum(file_name), " ");
            printf("SHA1 sum: %s\n",sha1sum);
        }

        if(strcmp(hashes[i],"sha256")==0) {
            sha256sum = strtok(sha256_sum(file_name)," ");
            printf("SHA256 sum: %s\n",sha256sum);
        }        
    }

    
    //process the command

    //call outside functions
    
    //free alocated memory
    {
        free(dir_name);
        free(file_name);
        free(outfile);
        free(md5sum);
        free(sha1sum);
        free(sha256sum);
    }
    return 0;
}