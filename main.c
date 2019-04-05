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
#include "file_logging.h"
#include "hash_functions.h"
#include "recursive_forensic.h"
#include "file_forensic.h"
#include "signal_handlers.h"

int stdout_save;
int fd;

void show_usage() {
    printf("Usage: forensic [-h [md5[,sha1[,sha256]]] [-o <outfile>] [-r] <file|dir>  [-v]\n");
}

// void display_info(struct Contents * contents) {
//     //first check for recursive analysis on directories
//     if(contents->dir_name != NULL) {
//         printf("Directory name: %s\n",contents->dir_name);
//     }   
//     else if(contents->file_name != NULL) {
//         printf("File name: %s\n",contents->file_name);
//     }
//     else {
//         printf("File name missing.\n");
//         show_usage();
//         return;
//     }

//     //check if the hash option has been selected
//     if(contents->md5_hash) {
//         printf("MD5 selected.\n");
//     }

//     if(contents->sha1_hash) {
//         printf("SHA1 selected.\n");
//     }

//     if(contents->sha256_hash) {
//         printf("SHA256 selected.\n");
//     }

//     //check if the outfile option has been selected
//     if(contents->outfile != NULL) {
//         printf("Output file: %s\n",contents->outfile);
//     }

//     //check if the execution log option has been selected
//     if(contents->log_check) {
//         init_time();
//         verbose_command(getpid(),contents);
//         printf("Logging execution events.\n");
//     } 
//     else {
//         printf("Not logging execution events.\n");
//     }
// }

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

    //Just display the info collected after parsing;
    //display_info(&cont);

    if(cont.outfile != NULL) {
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

    if(cont.dir_name != NULL) {
        int return_value = 0;
        char curr_path[MAX_BUF] = "";
        strcpy(curr_path,cont.dir_name);

        //fazer fork tal que pai le os sinais e o filho faz chamada recursiva
        pid_t pid = fork();

        if(pid > 0){
            if(cont.outfile != NULL)
                subscribeSIGUSR();
        }
        else if (pid == 0){
            if((return_value = recursive_forensic(curr_path,&cont)) !=0) {
                perror(curr_path);
            }            
        }

        int status;
        while(wait(&status) > 0);
        // if(cont.outfile != NULL) {
        //     close(fd);
        //     close(stdout_save);
        // }
    }

    if(cont.file_name != NULL) {
        char* result = calloc(MAX_BUF,1);
    
        if(file_forensic(cont.file_name, &cont, result) != 0){
            perror("Forensic Error\n");
        }

        printf("%s\n", result);

        free(result);
    }

    return 0;
}

