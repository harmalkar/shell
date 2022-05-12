/**
 * @brief Header file for a shell which parses and executes user-input commands
 * 
 * Siddhartha Harmalkar
 * May 2022
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_INPUT_SIZE          1000
#define MAX_COMMANDLINE_ARGS    100


/**
 * @brief Parse an input string into command line arguments
 * 
 * @param s The input string
 * @param arguments The list of arguments (must be allocated prior to use)
 * @return int Error code: 0 for successful parse, -1 for mismatched quotes, -2 for allocation error
 */
int parse(char* s, char** arguments);