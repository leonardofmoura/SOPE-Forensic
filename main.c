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
#include "hash_functions.h"
#include "recursive_forensic.h"

//include de ficheiros de fora
#include "file_forensic.h"
#include "signal_handlers.h"

bool o_flag;

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
    if(contents->log_check) {
        init_time();
        verbose_command(getpid(),contents);
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

    install_SIGINT_handler();
    
    //parse the command given
    struct Contents cont;
    memset(&cont,0,sizeof(cont));
    //cont = malloc(sizeof(struct Contents)); // [file,folder,hashes,outfile,log]   
    if(input_parser(argc,argv,&cont) != 0) {
        printf("Invalid option or wrong number of arguments.\n");
        show_usage();
        return 2;
    }

    //initialize file logging
    if (cont.log_check) {
        init_time();
        verbose_command(getpid(),&cont);
    }

    //initialize signal handler struct
    initializeActionStruct();

    //Just display the info collected after parsing;
    //display_info(&cont);

    int fd;

    int stdout_save;

    if(cont.outfile != NULL) {
        o_flag = true; 

        fd = open(cont.outfile, O_WRONLY | O_CREAT | O_TRUNC,0644);
        if(fd == -1) {
            perror(cont.outfile);
            close(fd);
            return 4;
        }
        //get std out descriptor and save it in a temporary variable
        stdout_save = dup(STDOUT_FILENO);

        //activate sigusr1
        printf("Data saved on file %s\n",cont.outfile);
        dup2(fd,STDOUT_FILENO);
        close(fd);
    }
    else o_flag = false;

    if(cont.dir_name != NULL) {
        int return_value = 0;
        char curr_path[MAX_BUF] = "";
        strcpy(curr_path,cont.dir_name);
        if((return_value = recursive_forensic(curr_path,&cont)) !=0) {
            perror(curr_path);
            return return_value;
        }
        
        return 0;
    }

    if(cont.file_name != NULL) {
        char* result = calloc(MAX_BUF,1);
    
        if(file_forensic(cont.file_name, &cont, result) != 0){
            perror("Forensic Error\n");
        }

        printf("%s\n", result);

        free(result);
    }

    if(o_flag) {
        dup2(stdout_save, STDOUT_FILENO);
        close(stdout_save);
        printf("Directories analysed: %d\nFiles analysed: %d\n", getCounter_SIGUSR1(), getCounter_SIGUSR2());
    }

    return 0;
}