#define MAX_BUFFER 256


char* md5_sum(const char* file_name);

char* sha1_sum(const char* file_name);

char* sha256_sum(const char* file_name);

//verify if the hash is a valid one. I.e, if its either MD5, SHA1 or SHA256
int check_hash(char* hash);
