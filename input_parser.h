#ifndef _INPUT_PARSER_H_
#define _INPUT_PARSER_H_

struct Contents {

    char* file_name;

    char* dir_name;

    char* outfile;

    char* hashes[4];

    char* log_check;

};

int input_parser(int n_args, char* argv[],struct Contents *contents);

int hashes_parser(char* hash_string, char* hashes []);

#endif
