#include "shell.h"

/**
 * @brief This program creates a shell which parses and executes user-input commands
 * 
 * Siddhartha Harmalkar
 * May 2022
 * 
 */

int main() {
    char input[MAX_INPUT_SIZE];
    char* arguments[MAX_COMMANDLINE_ARGS+1];
    int i, c;

    while (1) {
        // Prompt for input
        fprintf(stderr, "$ ");

        // Read input from user, exiting if EOF is received (ctrl+D)
        if (!fgets(input, MAX_INPUT_SIZE, stdin))
            return 0;

        // Check if input is too long
        if (input[strlen(input)-1] != '\n'){
            fprintf(stderr, "error: input was too large (max input size is %d characters)\n", MAX_INPUT_SIZE);

            // Clear input buffer to avoid reading in the rest of the input during the next iteration
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        // Parse input into command line arguments
        input[strlen(input) - 1] = '\0';                            // Remove newline
        int parse_error = parse((char*)(input), arguments);

        // Check if parse was successful
        if (parse_error == -1){
            fprintf(stderr, "error: mismatched quotes\n");
        }
        else if (parse_error == -2){
            fprintf(stderr, "error: too many command line arguments (maximum of %d allowed)\n", MAX_COMMANDLINE_ARGS);
        }
        else if (parse_error == -3){
            fprintf(stderr, "error: allocation error while parsing\n");
        }

        // Execute desired command
        else{
            // No input given: Do nothing
            if (!arguments[0]){
                continue;
            }

            // Exit shell if requested
            else if (!strcmp(arguments[0], "exit")){
                return 0;
            }

            // Change directory command requires special handling to maintain directory between prompts
            else if (!strcmp(arguments[0], "cd")){
                // Validate command
                for(i = 0; arguments[i]; i++);
                if (i < 2)
                    fprintf(stderr, "error: too few arguments given to cd (usage: cd <path>)\n");
                else if (i > 2)
                    fprintf(stderr, "error: too many arguments given to cd (usage: cd <path>)\n");

                // Attempt to change directory
                else {
                    if (chdir(arguments[1])){
                        fprintf(stderr, "error: cd failed\n");
                    }
                }
            }

            // General case: Create a new process to handle the desired command
            else{
                pid_t pid = fork();
                if (pid == 0){
                    execvp(arguments[0], arguments);
                    fprintf(stderr, "error: failed to execute the command (type 'exit' to exit shell)\n"); 
                    exit(0);
                }
                else if (pid < 0){
                    fprintf(stderr, "error: could not create child process to execute command");
                }
                else {
                    int status;
                    waitpid(pid, &status, 0);
                    if (WEXITSTATUS(status)){
                        fprintf(stderr, "error: command exited with code %d\n", WEXITSTATUS(status));
                    }
                }
            }
        }
    }

    return -1; //Control should never reach here
}

int parse(char* s, char** arguments){
    // Empty input: No arguments
    if (strlen(s) == 0){
        arguments[0] = NULL;
        return 0;
    }

    // Variables to store current argument being parsed, integers for indexing, and quotation mark type
    char* argument;
    int  i = 0, j = 0, k = 0;
    char matching_quote;

    // Allocate space for argument on stack
    argument = (char*) calloc(strlen(s) + 1, sizeof(char));
    if (!argument)
        return -3;

    // Search through input for arguments to parse
    for(i = 0; s[i]; i++){
        // Parse argument if next argument is a delimiter (space or quote)
        if (s[i] == '"' || s[i] == '\'' || s[i] == ' '){
            // If next character is a a quote, look for matching quote first
            // Quotes are mismatched if the end of the string is reached before matching quote is found
            if (s[i] == '"' || s[i] == '\''){
                matching_quote = s[i];
                i++;
                while(s[i] && s[i] != matching_quote)
                    argument[j++] = s[i++];
                if (!s[i]){
                    free(argument);
                    return -1;
                }
            }

            // Add argument to list of arguments
            argument[j] = '\0';
            j = 0;
            if (strlen(argument) > 0){
                if (k >= MAX_COMMANDLINE_ARGS){
                    free(argument);
                    return -2;
                }
                arguments[k++] = strdup(argument);
            }
            free(argument);
            argument = (char*) calloc(strlen(s) - i + 1, sizeof(char));
            if (!argument)
                return -3;
        }

        // Otherwise, continue to look for next argument
        else{
            argument[j++] = s[i];
        }
    }

    // Parse the last argument
    argument[j] = '\0';
    if (strlen(argument) > 0){
        if (k >= MAX_COMMANDLINE_ARGS){
            free(argument);
            return -2;
        }
        arguments[k++] = strdup(argument);
    }
    free(argument);

    // Null-terminate the arguments list
    arguments[k] = NULL;
    return 0;
}



