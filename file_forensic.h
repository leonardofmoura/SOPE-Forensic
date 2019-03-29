#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <input_parser.h>

#define MAX_BUF     512

char * getFileInfo(const char * file_name);
char* selectPermissions(mode_t mode);
char * getFileStatus(const char* file_name);
char* getFileHash(const char* file_name, char* hashes[]);
int file_forensic(const char* file_name, struct Contents* contents);
