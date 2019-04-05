#include "signal_handlers.h"
#include "input_parser.h"
#include "file_logging.h"

int stdout_save;
static int sigusr1_counter = 0;
static int sigusr2_counter = 0;
char * filename;
static bool sigint = false;

void output_filename(char* name){
    filename = name;
}

//handlers code
void SIGUSR1_handler(int d){
    verbose_signal(getpid(),d);
    sigusr1_counter++;
    verbose_signal(getpid(),SIGUSR1);
}

void SIGUSR2_handler(int d){
    // verbose_signal(getpid(),d);
    sigusr2_counter++;
    //verbose_signal(getpid(),SIGUSR2);
    dup2(stdout_save, STDOUT_FILENO);
    close(stdout_save);    

    printf("New directory: %d/%d directories/files at this time.\n", sigusr1_counter, sigusr2_counter);
}

void SIGINT_handler(int sig) {
    verbose_signal(getpid(),sig);
    sigint = true;
}

bool get_sigint() {
    return sigint;
}

void subscribeSignal(int SIGNAL, void (*func)(int), int FLAG){
    struct sigaction action;
    action.sa_handler = func;
    sigemptyset(&action.sa_mask);
    action.sa_flags = FLAG;

    if (sigaction(SIGNAL, &action, NULL) < 0)
    {
        fprintf(stderr,"Unable to install %d handler\n", SIGNAL);
        exit(1);
    }
}

void subscribeSIGUSR(){
    subscribeSignal(SIGUSR1, SIGUSR1_handler, SA_RESTART);

    subscribeSignal(SIGUSR2, SIGUSR2_handler, SA_RESTART);
}

void install_SIGINT_handler() {
    subscribeSignal(SIGINT, SIGINT_handler, SA_RESTART);
}

