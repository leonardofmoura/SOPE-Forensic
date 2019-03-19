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
//include de ficheiros de fora

void show_usage() {
    printf("Usage: forensic [-r] [-h [md5[,sha1[,sha256]]] [-o <outfile>] [-v] <file|dir>\n");
}

int proc_args(char* opts[],char* dir_name, char* file_name, char* hashes[],char* outfile) {
    //first check for recursive analysis on directories
    if(opts[1] != NULL) {

        dir_name = opts[1];
        printf("Directory name: %s\n",dir_name);
    }   
    else if(opts[0] != NULL) {

        file_name = opts[0];
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

        for(i=0; i <4; i++) {
            if(hashes[i] == NULL)
                break;
            else {
                printf("Hash %d: %s\n",i+1,hashes[i]);
            }
        }


    }

    //check if the outfile option has been selected
    if(opts[3] != NULL) {
        outfile = opts[3];
        printf("Output file: %s\n",outfile);
    }

    //check if the execution log option has been selected
    if(strcmp(opts[4],"true")== 0) {
        printf("Logging execution events.\n");
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

    char* dir_name = NULL;
    char* file_name = NULL;
    char * hashes[4] = {NULL};
    char* outfile = NULL;

    //process parsed and check for missing info
    if (proc_args(opts,dir_name,file_name,hashes,outfile) !=0) {
        show_usage();
        return 3;
    };

    printf("%s %s %s\n",hashes[0],file_name,outfile);
    if(hashes[0] != NULL && file_name != NULL) {
        if(check_hash(hashes[0]) != 0) {
            printf("Invalid hash choosen.\n");
            show_usage();
            return 4;
        }

        printf("File hash: %s",hash_function(hashes[0],file_name));
    }
    //process the command

    //call outside functions

    return 0;
}