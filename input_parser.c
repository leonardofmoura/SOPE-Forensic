#include <stdio.h>
#include <string.h>
#include <input_parser.h>
#include <hash_functions.h>

//input parser function
int input_parser(int n_args, char* argv[],struct Contents *contents) {
    
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
                        hashes_parser(argv[i+1],contents);
                        needed +=2;
                        break;
                    case 'v':
                        contents->log_check = true;
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

void hashes_parser(char* hash_string,struct Contents* contents) {
    
    if(strstr(hash_string,"md5") != NULL) {
        contents->md5_hash = true;
    }

    if(strstr(hash_string,"sha1") != NULL) {
        contents->sha1_hash = true;
    }
    
    if(strstr(hash_string,"sha256") != NULL) {
        contents->sha256_hash = true;
    }
}