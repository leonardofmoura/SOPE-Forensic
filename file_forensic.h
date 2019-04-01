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

void getDate(time_t * date_time,char* date);
void getFileInfo(char * file_name, char* info);
void getFileStatus(char* file_name, char* info);
void getFileHash(char* file_name, struct Contents* contents, char* info);
int file_forensic(char* file_name, struct Contents* contents, char* result);
