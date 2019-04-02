#pragma once

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void initializeActionStruct();
void subscribeSignal(int SIGNAL, void (*func)(void));
void unsubscribeSignal(int SIGNAL);
int getCounter_SIGUSR1();
int getCounter_SIGUSR2();