#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <file_logging.h>

static time_t starting_time;

//stores the staring time of the program
void init_time() {
    time(&starting_time);
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

//logging functions

//start logging

//writing logging

//finish logging

//time tracking functions

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
    char time_string[15];
    calculate_elapsed_time(time_string);

    //convert the pid to string
    char pid_string[15];
    pid_to_string(pid, pid_string);

    //write the pid
    for (int i = 0; i < 15; i++) {
        if (pid_string[i] == '\0') {
            break;
        }
        write(logfile,&pid_string[i],sizeof(char));
    }

    //write the time
    for (int i = 0; i < 15; i++) {
        if (time_string[i] == '\0') {
            break;
        }
        write(logfile,&time_string[i],sizeof(char));
    }

    close(logfile);
}

