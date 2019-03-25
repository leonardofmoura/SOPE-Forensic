#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hash_functions.h>


char* md5_sum(const char* file_name) {
    char* hash_command = malloc(MAX_BUFFER);
    strcpy(hash_command,"md5sum ");
    strcat(hash_command,file_name);
    
    FILE* f = popen(hash_command,"r");
    char* hash = malloc(MAX_BUFFER);
    
    if(fread(hash,1,MAX_BUFFER,f) <0) {
        perror(hash);
        exit(1);
    }
    
    pclose(f);
    free(hash_command);
    return hash;
}

char* sha1_sum(const char* file_name) {
    char* hash_command = malloc(MAX_BUFFER);
    strcpy(hash_command,"sha1sum ");
    strcat(hash_command,file_name);
    
    FILE* f = popen(hash_command,"r");
    char* hash = malloc(MAX_BUFFER);

    if(fread(hash,1,MAX_BUFFER,f) <0) {
        perror(hash);
        exit(1);
    }
    pclose(f);
    free(hash_command);
    return hash;
}

char* sha256_sum(const char* file_name) {
    char* hash_command = malloc(MAX_BUFFER);
    strcpy(hash_command,"sha256sum ");
    strcat(hash_command,file_name);
    
    FILE* f = popen(hash_command,"r");
    char* hash = malloc(MAX_BUFFER);
    if(fread(hash,1,MAX_BUFFER,f) <0) {
        perror(hash);
        exit(1);
    }
    pclose(f);
    free(hash_command);
    return hash;
}

int check_hash(char* hash) {
    return strcmp(hash,"md5") || strcmp(hash,"sha1") || strcmp(hash,"sha256");
}