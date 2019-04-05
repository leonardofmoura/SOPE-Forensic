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

void parse_commands(struct Contents *contents, int argc, char* argv[]){
    memset(contents,0,sizeof(*contents));
    //contents = malloc(sizeof(struct Contents)); // [file,folder,hashes,outfile,log] 

    if(input_parser(argc,argv,contents) != 0) {
        printf("Invalid option or wrong number of arguments.\n");
        show_usage();
        exit(2);
    }
}

void output_file_active(struct Contents * contents){
    fd = open(contents->outfile, O_WRONLY | O_CREAT | O_TRUNC,0644);

    if(fd == -1) {
        perror(contents->outfile);
        close(fd);
        exit(4);
    }
    //get std out descriptor and save it in a temporary variable
    stdout_save = dup(STDOUT_FILENO);

    //activate sigusr1
    printf("Data saved on file %s\n",contents->outfile);
    dup2(fd,STDOUT_FILENO);
    close(fd);
}

void recursive_analysis(struct Contents * contents){
    int return_value = 0;
    char curr_path[MAX_BUF] = "";
    strcpy(curr_path,contents->dir_name);

    //fazer fork tal que pai le os sinais e o filho faz chamada recursiva
    pid_t pid = fork();

    if(pid > 0){
        if(contents->outfile != NULL)
            subscribeSIGUSR();
    }
    else if (pid == 0){
        if((return_value = recursive_forensic(curr_path, contents)) !=0) {
            perror(curr_path);
        }            
    }

    int status;
    while(wait(&status) > 0);
}

void single_file_analysis(struct Contents * contents){
    char* result = calloc(MAX_BUF,1);

    if(file_forensic(contents->file_name, contents, result) != 0){
        perror("Forensic Error\n");
    }

    printf("%s\n", result);

    free(result);
}

int main(int argc, char* argv[]) {

    if(argc == 1) {
        //invalid arguments
        show_usage();
        exit(1);
    }

    install_SIGINT_handler();
    
    //parse the command given
    struct Contents contents;
    parse_commands(&contents, argc, argv);

    //initialize file logging
    if (contents.log_check) {
        init_time();
        verbose_command(getpid(),&contents);
    }

    //Just display the info collected after parsing;
    //display_info(&contents);

    if(contents.outfile != NULL) {
        output_file_active(&contents);
    }

    if(contents.dir_name != NULL) {
        recursive_analysis(&contents);
    }

    if(contents.file_name != NULL) {
        single_file_analysis(&contents);
    }

    return 0;
}

