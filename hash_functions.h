#pragma once

#define MAX_BUFFER 256

void md5_sum(const char* file_name,char * hash);

void sha1_sum(const char* file_name,char * hash);

void sha256_sum(const char* file_name, char * hash);

//verify if the hash is a valid one. I.e, if its either MD5, SHA1 or SHA256
int check_hash(char* hash);