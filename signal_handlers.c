#include "signal_handlers.h"

struct sigaction action;
int sigusr1_counter;
int sigusr2_counter;

//handlers code


//install handlers
void initializeActionStruct(){
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
}

void subscribeSignal(int SIGNAL){

    
    sigaction(SIGNAL, &action, NULL);
}

void unsubscribeSignal(int SIGNAL){
    
}

int getCounter_SIGUSR1(){
    return sigusr1_counter;
}

int getCounter_SIGUSR2(){
    return sigusr2_counter;
}

