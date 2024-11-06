
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <simple-cypher/simple-cypher.h>

static inline int parse_uint_as_hexadecimal (const char *str, uintmax_t *valuep){
  uintmax_t value = 0;
  for (size_t index = 0; index < SIZE_MAX; index++){
    if ('0' <= str[index] && str[index] <= '9'){
      value *= 16;
      value += str[index] - '0';
    }
    else 
    if ('a' <= str[index] && str[index] <= 'f'){
      value *= 16;
      value += str[index] - 'a' + 16;
    }
    else 
    if ('A' <= str[index] && str[index] <= 'F'){
      value *= 16;
      value += str[index] - 'A' + 16;
    }
    else 
    if ('\0' == str[index]){
      break;
    }
    else {
      fprintf(stderr, "Illegal character '%c' detected at parsing hexadecimal.\n", str[index]);
      return 1;
    }
  }
  *valuep = value;
  return 0;
}

static inline int parse_uint_as_digits (const char *str, uintmax_t *valuep){
  uintmax_t value = 0;
  for (size_t index = 0; index < SIZE_MAX; index++){
    if ('0' <= str[index] && str[index] <= '9'){
      value *= 10;
      value += str[index] - '0';
    }
    else 
    if ('\0' == str[index]){
      break;
    }
    else {
      fprintf(stderr, "Illegal character '%c' detected at parsing digits.\n", str[index]);
      return 1;
    }
  }
  *valuep = value;
  return 0;
}

static inline int parse_uint (const char *str, uintmax_t *valuep){
  if (str[0] == '0'){
    if (str[1] == 'x'){
      return parse_uint_as_hexadecimal(str +2, valuep);
    }
    else {
      return parse_uint_as_digits(str, valuep);
    }
  }
  else {
    return parse_uint_as_digits(str, valuep);
  }
}

static inline int parse_args (int argc, char **argv, FILE **inputp, FILE **outputp, uint64_t *keyp, bool *showhelpp, bool *showversionp){
  char *inputfile = NULL;
  char *outputfile = NULL;
  uint64_t key; 
  bool givenkey = false;
  bool showhelp = false; 
  bool showversion = false;
  size_t index = 1;
  while (index < argc){
    if (strcmp(argv[index], "--help") == 0){
      showhelp = true;
      break;
    }
    else 
    if (strcmp(argv[index], "--version") == 0){
      showversion = true;
      break;
    }
    else 
    if (strcmp(argv[index], "-k") == 0){
      if (index +1 < argc){
        if (!givenkey){
          if (parse_uint(argv[index +1], &key)){
            return 1;
          }
          givenkey = true;
          index += 2;
        }
        else {
          fprintf(stderr, "Already given a parameter -k \"%s\".\n", argv[index +1]);
          return 1;
        }
      }
      else {
        fprintf(stderr, "Missing value for a parameter -k.\n");
        return 1;
      }
    }
    else 
    if (strcmp(argv[index], "-o") == 0){
      if (index +1 < argc){
        if (outputfile == NULL){
          outputfile = argv[index +1];
          index += 2;
        }
        else {
          fprintf(stderr, "Already given a output file \"%s\".\n", argv[index +1]);
          return 1;
        }
      }
      else {
        fprintf(stderr, "Missing value for a parameter -o.\n");
        return 1;
      }
    }
    else {
      if (inputfile == NULL){
        inputfile = argv[index];
        index += 1;
      }
      else {
        fprintf(stderr, "Already given a input file \"%s\".\n", argv[index]);
        return 1;
      }
    }
  }
  if (showhelp){
    *inputp = stdin;
    *outputp = stdout;
    *showhelpp = true;
    *showversionp = false;
    return 0;
  }
  else 
  if (showversion){
    *inputp = stdin;
    *outputp = stdout;
    *showhelpp = false;
    *showversionp = true;
    return 0;
  }
  else {
    if (givenkey){
      FILE *input;
      if (inputfile == NULL){
        input = stdin;
      }
      else {
        input = fopen(inputfile, "rb");
        if (input == NULL){
          fprintf(stderr, "Could not open file \"%s\" (errno = %d).\n", inputfile, errno);
          return 1;
        }
      }
      FILE *output;
      if (outputfile == NULL){
        output = stdout;
      }
      else {
        output = fopen(outputfile, "wb");
        if (output == NULL){
          fprintf(stderr, "Could not open file \"%s\" (errno = %d).\n", outputfile, errno);
          return 1;
        }
      }
      *inputp = input;
      *outputp = output;
      *keyp = key;
      *showhelpp = false;
      *showversionp = false;
      return 0;
    }
    else {
      fprintf(stderr, "Require a -k parameter.\n");
      return 1;
    }
  }
}

const char HELP_MESSAGE[] = 
"Usage: simple-cypher [OPTION]... -k KEY [FILE]\n"
"Encrypt or decrypt FILE with simple-cypher.\n"
"\n"
"Options:\n"
"  -o file: An output file. default is stdout.\n"
"  --help: Show help then exit.\n"
"  --version: Show version then exit.\n"
"\n"
"Arguments:\n"
"  KEY: An unsigned integer used to common key. You can use digits or hexadecimal format like as 123 and 0x123abc.\n"
"  FILE: An input file. default is stdin.\n"
;

const char VERSION_MESSAGE[] = 
"simple-cypher 0.1.0\n"
;

int main (int argc, char **argv){
  FILE *input;
  FILE *output;
  uint64_t key;
  bool showhelp;
  bool showversion;
  if (parse_args(argc, argv, &input, &output, &key, &showhelp, &showversion)){
    return 1;
  }
  if (showhelp){
    printf("%s", HELP_MESSAGE);
    return 0;
  }
  else 
  if (showversion){
    printf("%s", VERSION_MESSAGE);
    return 0;
  }
  else {
    simple_cypher cypher;
    simple_cypher_setup(key, &cypher);
    while (true){
      uint8_t readdata[4096];
      size_t readsize = fread(readdata, 1, sizeof(readdata), input);
      if (readsize < sizeof(readdata)){
        if (ferror(input)){
          fprintf(stderr, "Caused some error at reading a file.\n");
          return 1;
        }
        if (readsize == 0){
          break;
        }
      }
      simple_cypher_encrypt_decrypt(readdata, readsize, &cypher, readdata);
      size_t writtensize = fwrite(readdata, 1, readsize, output);
      if (writtensize < readsize){
        if (ferror(output)){
          fprintf(stderr, "Caused some error at writing into a file.\n");
          return 1;
        }
      }
    }
    return 0;
  }
}
