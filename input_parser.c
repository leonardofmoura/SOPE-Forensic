#include <string.h>

//input parser function
int input_parser(int n_args, char* argv[],char* options[]) {
    
    options[4] = "false";

    for(int i = 1; i< n_args; i++) {
        if( i ==1 && strcmp(argv[i],"-r") != 0) {
            options[0] = argv[i];
        }

        if(strncmp(argv[i],"-",1) ==0) {
            if(strlen(argv[i]) == 2) {

                switch(argv[i][1]) {
                    case 'r':
                        options[1] = argv[i+1];
                        break;
                    case 'h':
                        options[2] = argv[i+1];
                        break;
                    case 'v':
                    options[4] = "true";
                        break;
                    case 'o':
                        options[3] = argv[i+1];
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

    return 0;
}