#pragma once

#include <stdbool.h>

#define MAX_BUFFER 256

struct Contents {

    char* file_name;

    char* dir_name;

    char* outfile;

    bool md5_hash;

    bool sha1_hash;

    bool sha256_hash;

    bool log_check;
};

int input_parser(int n_args, char* argv[],struct Contents *contents);

void hashes_parser(char* hash_string, struct Contents *contents);
