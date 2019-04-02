#include "signal_handlers.h"

struct sigaction action;
int sigusr1_counter;
int sigusr2_counter;

//handlers code
void SIGUSR1_handler(){
    sigusr1_counter++;
}

void SIGUSR2_handler(){
    sigusr2_counter++;
}

//install handlers
void initializeActionStruct(){
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
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

