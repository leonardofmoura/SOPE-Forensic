#pragma once

#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <wait.h>
#include <fcntl.h>

void install_SIGINT_handler();

bool get_sigint();

#define NO_FLAG     0

void subscribeSignal(int SIGNAL, void (*func)(int), int FLAG);
void unsubscribeSignal(int SIGNAL);
void subscribeSIGUSR();
void printMsg();
