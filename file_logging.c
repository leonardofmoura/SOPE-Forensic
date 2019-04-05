#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <signal.h>
#include <string.h>
#include <sys/file.h>
#include "input_parser.h"
#include "file_logging.h"

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

void get_signal_string(int sig, char signal_string[]) {
    sprintf(signal_string,"SIGNAL : %s\n", strsignal(sig));
}

void get_command_string(struct Contents* contents, char command_string[]) {
    sprintf(command_string,"COMMAND forensic ");
   
    if (contents->dir_name != NULL) {
        command_string = strcat(command_string,"-r ");
        command_string = strcat(command_string,contents->dir_name);
        command_string = strcat(command_string," ");
    }
    if (contents->md5_hash || contents->sha1_hash || contents->sha256_hash) {
        command_string = strcat(command_string,"-h ");
        if (contents->md5_hash) {
            command_string = strcat(command_string,"md5");;
            if (contents->sha1_hash || contents->sha256_hash) {
                command_string = strcat(command_string,",");
            }
            else {
                command_string = strcat(command_string," ");
            }
        }
        if (contents->sha1_hash) {
            command_string = strcat(command_string,"sha1");
            if (contents->sha256_hash) {
                command_string = strcat(command_string,",");
            }
            else {
                command_string = strcat(command_string," ");
            }
        }
        if (contents->sha256_hash) {
            command_string = strcat(command_string,"sha256");
            command_string = strcat(command_string," ");
        }
    }
    if (contents->outfile != NULL) {
        command_string = strcat(command_string,"-o ");
        command_string = strcat(command_string,contents->outfile);
        command_string = strcat(command_string," ");
    }
    if (contents->log_check) {
        command_string = strcat(command_string,"-v ");
    }
    if (contents->file_name != NULL) {
        strcat(command_string,contents->file_name);
        strcat(command_string,"\n");
    }
}

int write_string_to_file(char string[], int file) {
    for (int i = 0; i < MAX_STR_SIZE; i++) {
        if (string[i] == '\0') {
            break;
        }
        if (write(file,&string[i],sizeof(char))== -1) {
            return 1;
        }
    }

    return 0;
}


void verbose_command(pid_t pid, struct Contents* contents) {
    if (verbose) {
        //initialize log file
        char* logfilename = getenv("LOGFILENAME");
        int logfile = open(logfilename, O_WRONLY | O_CREAT | O_TRUNC,0644); 

        //get the time 
        char time_string[MAX_STR_SIZE];
        calculate_elapsed_time(time_string);

        //convert the pid to string
        char pid_string[MAX_STR_SIZE];
        pid_to_string(pid, pid_string);

        //assemble the command 
        char command_string[MAX_STR_SIZE];
        get_command_string(contents,command_string);

        //write the time
        if (write_string_to_file(time_string,logfile) != 0) {
            fprintf(stderr,"Unable to write time string to file\n");
        }

        //write the pid
        if (write_string_to_file(pid_string,logfile) != 0) {
            fprintf(stderr,"Unable to write pid string to file\n");
        }

        if (write_string_to_file(command_string,logfile) != 0) {
            fprintf(stderr,"Unable to write command string to file\n");
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

        flock(logfile,LOCK_EX);

        //write the time
        if (write_string_to_file(time_string,logfile) != 0) {
            fprintf(stderr,"Unable to write time string to file\n");
        }

        //write the pid
        if (write_string_to_file(pid_string,logfile) != 0) {
            fprintf(stderr,"Unable to write pid string to file\n");
        }

        if (write_string_to_file(analized_string,logfile) != 0) {
            fprintf(stderr,"Unable to write analized string to file\n");
        }

        flock(logfile,LOCK_UN);

        close(logfile); 
    }
    else {
        return;
    }      
}

void verbose_signal(pid_t pid, int sig) {
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

        //process the signal to a string
        char signal_string[MAX_STR_SIZE];
        get_signal_string(sig,signal_string);

        flock(logfile,LOCK_EX);

        //write the time
        if (write_string_to_file(time_string,logfile) != 0) {
            fprintf(stderr,"Unable to write time string to file\n");
        }

        //write the pid
        if (write_string_to_file(pid_string,logfile) != 0) {
            fprintf(stderr,"Unable to write pid string to file\n");
        }

        if (write_string_to_file(signal_string,logfile) != 0) {
            fprintf(stderr,"Unable to write signal string to file\n");
        }

        flock(logfile,LOCK_UN);


        close(logfile); 
    }
    else {
        return;
    }    
}
