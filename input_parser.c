#include <stdio.h>
#include <string.h>
#include <input_parser.h>

//input parser function
int input_parser(int n_args, char* argv[],struct Contents *contents) {
    
    contents->log_check = "false";

    //number of arguments needed to make up a valid command
    int needed=2;

    for(int i = 1; i< n_args; i++) {
        if( i ==1 && strcmp(argv[i],"-r") != 0) {
            contents->file_name = argv[n_args-1];
        }

        if(strncmp(argv[i],"-",1) ==0) {
            if(strlen(argv[i]) == 2) {

                switch(argv[i][1]) {
                    case 'r':
                        contents->dir_name = argv[i+1];
                        needed++;
                        break;
                    case 'h':
                        if(hashes_parser(argv[i+1],contents->hashes) != 0) {
                            return -1;
                        }
                        needed +=2;
                        break;
                    case 'v':
                        contents->log_check = "true";
                        needed++;
                        break;
                    case 'o':
                        contents->outfile = argv[i+1];
                        needed +=2;
                        break;
                    default:
                        return -1;
                }
            }
        }
        else {
            continue;
        }
    }

    if(needed != n_args) {
        return -1;
    }

    return 0;
}

int hashes_parser(char* hash_string, char* hashes []) {
    hashes[0] = strtok(hash_string,","); //break the string into tokens
    int i =1;
    while((hashes[i] = strtok(NULL,",")) != NULL) {
        i++;
    }
    hashes[3] = NULL;
    
    i = 0;
    while(hashes[i] != NULL) {
        if((strcmp(hashes[i],"md5") != 0) &&
        (strcmp(hashes[i],"sha1") != 0) &&
        (strcmp(hashes[i],"sha256") != 0)) {
            return -1;
        }
        i++;
    }
    for(i = 0; i<4 ;i++) {
        if(hashes[i] == NULL)
            break;
    }
    return 0;
}