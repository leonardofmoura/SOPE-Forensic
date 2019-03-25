#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_BUF     1000

char * concatenate(char * old, char * new);
char * file(char * file_name);
char * getFileStatus(char* file_name);


