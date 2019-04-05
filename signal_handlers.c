#include "signal_handlers.h"
#include "input_parser.h"
#include "file_logging.h"
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <wait.h>

// extern int stdout_save;
// extern int fd;
static int sigusr1_counter = 0;
static int sigusr2_counter = 0;

static bool sigint = false;

//handlers code
void SIGUSR1_handler(int sigusr1_counter){
    sigusr1_counter++;
}

void SIGUSR2_handler(int sigusr2_counter){
    sigusr2_counter++;
    
    // int aux = dup(STDOUT_FILENO);

    // dup2(stdout_save, STDOUT_FILENO);
    // close(stdout_save);

    printf("New directory: %d/%d directories/files at this time.\n", sigusr1_counter, sigusr2_counter);

    // dup2(aux, STDOUT_FILENO);
    // close(aux);
}

void SIGINT_handler(int sig) {
    verbose_signal(getpid(),SIGINT);
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

