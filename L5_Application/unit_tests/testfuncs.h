#ifndef TESTFUNCS_H
# define TESTFUNCS_H

#include <stdint.h>

uint8_t get_two(char *ptr);
float decode_long(char *bytecode, unsigned int start, unsigned int end);
float decode_lat(char *bytecode, unsigned int start, unsigned int end);

#endif