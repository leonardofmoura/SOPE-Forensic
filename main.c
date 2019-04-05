//include de ficheiros de fora
#include "file_logging.h"
#include "hash_functions.h"
#include "recursive_forensic.h"
#include "file_forensic.h"
#include "signal_handlers.h"

extern int stdout_save;

void show_usage() {
    printf("Usage: forensic [-h [md5[,sha1[,sha256]]] [-o <outfile>] [-r] <file|dir>  [-v]\n");
}

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
    output_filename(contents->outfile);

    int fd = open(contents->outfile, O_WRONLY | O_CREAT | O_TRUNC,0644);

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
    char curr_path[MAX_BUF] = "";
    strcpy(curr_path,contents->dir_name);

    //fazer fork tal que pai le os sinais e o filho faz chamada recursiva
    pid_t pid = fork();

    if(pid > 0){
        if(contents->outfile != NULL)
            subscribeSIGUSR();
    }
    else if (pid == 0){
        if(recursive_forensic(curr_path, contents) !=0) {
            perror(curr_path);
        }
        return;
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

    if(contents.outfile != NULL) {
        output_file_active(&contents);
    }

    if(contents.dir_name != NULL) {
        recursive_analysis(&contents);
    }
    else if(contents.file_name != NULL) {
        single_file_analysis(&contents);
    }

    return 0;
}

