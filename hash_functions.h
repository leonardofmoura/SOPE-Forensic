#define MAX_BUFFER 512

//Retrieve the hash for a file
char * hash_function(char* hash,char* file_name);

//verify if the hash is a valid one. I.e, if its either MD5, SHA1 or SHA256
int check_hash(char* hash);
