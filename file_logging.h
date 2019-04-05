#ifndef _FILE_LOGGER_H_
#define _FILE_LOGGER_H_

#define MAX_STR_SIZE 256

#include <unistd.h>
#include "input_parser.h"

//stores the time of the initialization of the program
void init_time();

//calculates the elapsed time and puts it in a readable format in time_string
void calculate_elapsed_time(char time_string[]);

//Coverts the pid into a readable format and puts it in pid_string
void pid_to_string(pid_t pid, char pid_string[]);

//put the compiled string in analized_string
void get_analized_string(char filename[], char analized_string[]);

//assemble the command typed and put it in the command_string
void get_command_string(struct Contents* contents, char command_string[]);

//verbose the typed command
void verbose_command(pid_t pid, struct Contents* contents);

//verbose an analized file
void verbose_analized(pid_t pid, char filename[]);

//verbose a received signal
void verbose_signal(pid_t pid, int sig);

//writes string[] in file; Suposes file is open
int write_string_to_file(char string[], int file);

#endif