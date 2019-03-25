#ifndef _FILE_LOGGER_H_
#define _FILE_LOGGER_H_

#define MAX_STR_SIZE 256

enum log_event {
    COMMAND = 0,
    SIGNAL,
    ANALIZED
};

//stores the time of the initialization of the program
void init_time();

//calculates the elapsed time and puts it in a readable format in time_string
void calculate_elapsed_time(char time_string[]);

//Coverts the pid into a readable format and puts it in pid_string
void pid_to_string(pid_t pid, char pid_string[]);

//put the compiled string in analized_string
void get_analized_string(char filename[], char analized_string[]);

//verbose an analized file
void verbose_analized(pid_t pid, char filename[]);

//base function not to be used
void file_logger(pid_t pid, enum log_event ev);

#endif