#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>
#include <input_parser.h>
//include de ficheiros de fora

#define BUFFER_SIZE 256

void show_usage() {
    printf("Usage: forensic [-r] [-h [md5[,sha1[,sha256]]] [-o <outfile>] [-v] <file|dir>\n");
}

int main(int argc, char* argv[]) {

    if(argc == 1) {
        //invalid arguments
        show_usage();
        exit(1);
    }
    
    //parse the command given
    char * opts[5] = {NULL}; // [file,folder,hashes,outfile,log]   
    if(input_parser(argc,argv,opts) != 0) {
        printf("Invalid option.\n");
        show_usage();
        exit(2);
    }

    char* dir_name;
    char* file_name;
    char * hashes[4];
    char* outfile;
    //first check for recursive analysis on directories
    if(opts[1] != NULL) {

        dir_name = opts[1];
        printf("Directory name: %s\n",dir_name);
        
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
                    printf("Hash: %s\n",hashes[i]);
                }
            }
        }
        
        //check if the outfile option has been selected
        if(opts[3] != NULL) {
            outfile = opts[3];
            printf("Output file: %s\n",outfile);
        }

        

    }
    else { //it a file to analyze

        //check the file name
        if(opts[0] == NULL) {
            printf("File name missing.\n");
            show_usage();
            exit(2);
        }
        
        file_name = opts[0];
        printf("File name: %s\n",file_name);

        if(opts[2] != NULL) {
            hashes[0] = strtok(opts[2],",");
            int i =1;
            while((hashes[i] = strtok(NULL,",")) != NULL) {
                i++;
            }
            hashes[3] = NULL;

            for(i=0; i <4; i++) {
                if(hashes[i] == NULL)
                    break;
                else {
                    printf("Hash: %s\n",hashes[i]);
                }
            }
        }

        if(opts[3] != NULL) {
            outfile = opts[3];
            printf("Output file: %s\n",outfile);
        }
    }

    //check if the execution log option has been selected
    if(strcmp(opts[4],"true")== 0) {
        printf("Logging execution events.\n");
    } 
    else {
        printf("Not logging execution events.\n");
    }

    //process the command

    //call outside functions

    exit(0);
}