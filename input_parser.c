#include <string.h>

//input parser function
int input_parser(int n_args, char* argv[], char* options[]) {
    
    options[4] = "false";

    //number of arguments needed to make up a valid command
    int needed=2;

    for(int i = 1; i< n_args; i++) {
        if( i ==1 && strcmp(argv[i],"-r") != 0) {
            options[0] = argv[n_args-1];
        }

        if(strncmp(argv[i],"-",1) ==0) {
            if(strlen(argv[i]) == 2) {

                switch(argv[i][1]) {
                    case 'r':
                        options[1] = argv[i+1];
                        needed++;
                        break;
                    case 'h':
                        options[2] = argv[i+1];
                        needed +=2;
                        break;
                    case 'v':
                        options[4] = "true";
                        needed++;
                        break;
                    case 'o':
                        options[3] = argv[i+1];
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