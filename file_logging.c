#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <file_logging.h>

static time_t starting_time;
static bool verbose = false;

void init_time() {
    time(&starting_time);
    verbose = true;
}

void calculate_elapsed_time(char time_string[]) {
    time_t curr_time;
    time(&curr_time);

    time_t elapsed_seconds = (curr_time - starting_time);
    
    int hours = (int) (elapsed_seconds/3600);
    int minutes = ((int) (elapsed_seconds / 60)) % 60;
    int seconds = elapsed_seconds % 60;

    sprintf(time_string, "%d:%d:%d - ", hours, minutes, seconds);
}

void pid_to_string(pid_t pid, char pid_string[]) {
    sprintf(pid_string,"%d - ",pid);
}

void get_analized_string(char filename[], char analized_string[]) {
    sprintf(analized_string, "ANALIZED %s\n",filename);
}

//work in progress
void verbose_command(pid_t pid, char* opts[]) {
    if (verbose) {
        //initialize log file
        char* logfilename = getenv("LOGFILENAME");
        int logfile = open(logfilename, O_WRONLY | O_APPEND); 

        //get the time 
        char time_string[MAX_STR_SIZE];
        calculate_elapsed_time(time_string);

        //convert the pid to string
        char pid_string[MAX_STR_SIZE];
        pid_to_string(pid, pid_string);

        //write the pid
        for (int i = 0; i < MAX_STR_SIZE; i++) {
            if (pid_string[i] == '\0') {
                break;
            }
            write(logfile,&pid_string[i],sizeof(char));
        }

        //write the time
        for (int i = 0; i < MAX_STR_SIZE; i++) {
            if (time_string[i] == '\0') {
                break;
            }
            write(logfile,&time_string[i],sizeof(char));
        }

        close(logfile);
    }
    else {
        return;
    }
}


void verbose_analized(pid_t pid, char filename[]) {
    if (verbose) {
        //initialize log file
        char* logfilename = getenv("LOGFILENAME");
        int logfile = open(logfilename, O_WRONLY | O_APPEND); 

        //get the time 
        char time_string[MAX_STR_SIZE];
        calculate_elapsed_time(time_string);

        //convert the pid to string
        char pid_string[MAX_STR_SIZE];
        pid_to_string(pid, pid_string);

        //get the analized string
        char analized_string[MAX_STR_SIZE];
        get_analized_string(filename,analized_string);

        //write the pid
        for (int i = 0; i < MAX_STR_SIZE; i++) {
            if (pid_string[i] == '\0') {
                break;
            }
            write(logfile,&pid_string[i],sizeof(char));
        }

        //write the time
        for (int i = 0; i < MAX_STR_SIZE; i++) {
            if (time_string[i] == '\0') {
                break;
            }
            write(logfile,&time_string[i],sizeof(char));
        }

        //write the analized string
        for (int i = 0; i < MAX_STR_SIZE; i++) {
            if (analized_string[i] == '\0') {
                break;
            }
            write(logfile,&analized_string[i],sizeof(char));
        }

        close(logfile); 
    }
    else {
        return;
    }      
}

//base function not to be used
void file_logger(pid_t pid, enum log_event ev) {
    init_time();

    //initialize log file
    char* logfilename = getenv("LOGFILENAME");
    int logfile = open(logfilename, O_WRONLY | O_APPEND); 

    //burn some time
    // srand(time(NULL));
    // int j = 35778;
    // int lol = 0;
    // while (lol != j) {
    //     lol = rand();
    // }

    //get the time 
    char time_string[MAX_STR_SIZE];
    calculate_elapsed_time(time_string);

    //convert the pid to string
    char pid_string[MAX_STR_SIZE];
    pid_to_string(pid, pid_string);

    //write the pid
    for (int i = 0; i < MAX_STR_SIZE; i++) {
        if (pid_string[i] == '\0') {
            break;
        }
        write(logfile,&pid_string[i],sizeof(char));
    }

    //write the time
    for (int i = 0; i < MAX_STR_SIZE; i++) {
        if (time_string[i] == '\0') {
            break;
        }
        write(logfile,&time_string[i],sizeof(char));
    }

    close(logfile);
}

