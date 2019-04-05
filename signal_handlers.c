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

struct sigaction action;
int sigusr1_counter;
int sigusr2_counter;

static bool sigint = false;

//handlers code
void SIGUSR1_handler(){
    sigusr1_counter++;
}

void SIGUSR2_handler(){
    sigusr2_counter++;
}

void SIGINT_handler(int sig) {
    verbose_signal(getpid(),SIGINT);
    sigint = true;
}

bool get_sigint() {
    return sigint;
}

//install handlers
void initializeActionStruct(){
    sigemptyset(&action.sa_mask);
}

void install_SIGINT_handler() {
    action.sa_handler = SIGINT_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = SA_RESTART;

    if (sigaction(SIGINT, &action, NULL) < 0) {
        fprintf(stderr,"Unable to install SIGINT handler\n");
        exit(1);
    }
}

void subscribeSignal(int SIGNAL, void (*func)(void)){
    action.sa_handler = func;

    if (sigaction(SIGNAL, &action, NULL) < 0)
    {
        fprintf(stderr,"Unable to install %d handler\n", SIGNAL);
        exit(1);
    }
}

void unsubscribeSignal(int SIGNAL){

}

void subscribeSIGUSR(){
    subscribeSignal(SIGUSR1, SIGUSR1_handler);
    sigusr1_counter = 0;

    subscribeSignal(SIGUSR2, SIGUSR2_handler);
    sigusr2_counter = 0;
}

//other
int getCounter_SIGUSR1(){
    return sigusr1_counter;
}

int getCounter_SIGUSR2(){
    return sigusr2_counter;
}

