#pragma once

#include <stdbool.h>

void install_SIGINT_handler();

bool get_sigint();

void initializeActionStruct();
void subscribeSignal(int SIGNAL, void (*func)(void));
void unsubscribeSignal(int SIGNAL);
int getCounter_SIGUSR1();
int getCounter_SIGUSR2();