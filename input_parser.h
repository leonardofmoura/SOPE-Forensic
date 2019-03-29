<<<<<<< HEAD
#ifndef _INPUT_PARSER_H_
#define _INPUT_PARSER_H_
=======
#pragma once

#include <stdbool.h>

#define MAX_BUFFER 256
>>>>>>> 65347ffb5a74ba0fa226fcafb4e03582b6480982

struct Contents {

    char* file_name;

    char* dir_name;

    char* outfile;

    bool md5_hash;

    bool sha1_hash;

    bool sha256_hash;

    char* log_check;

};

int input_parser(int n_args, char* argv[],struct Contents *contents);

<<<<<<< HEAD
int hashes_parser(char* hash_string, char* hashes []);

#endif
=======
void hashes_parser(char* hash_string, struct Contents *contents);
>>>>>>> 65347ffb5a74ba0fa226fcafb4e03582b6480982
