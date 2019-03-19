#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hash_functions.h>

char * hash_function(char* hash,char* file_name) {
    char* hash_command = "";
    strcat(hash_command,hash);
    printf("hash command 1: %s",hash_command);
    strcat(hash_command,"sum ");
    printf("hash command 2: %s",hash_command);
    strcat(hash_command,file_name);
    printf("hash command 3: %s",hash_command);
    FILE* f1 = popen(hash_command,"r");
    char* hash1 = NULL;
    if( fread(hash1,1,MAX_BUFFER,f1) <0) {
        perror(hash1);
        exit(1);
    }
    printf("hash1: %s",hash1);
    pclose(f1);

    return hash1;
}

int check_hash(char* hash) {
    return strcmp(hash,"md5") || strcmp(hash,"sha1") || strcmp(hash,"sha256");
}