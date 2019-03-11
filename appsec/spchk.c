/*
Spell checker: Implemented in C
Author: Theodore Kim

This program runs on the command line.
CMD format:
 - spchk <no-arguments> <no-parameters>: spell check will prompt the user for the string
 - spchk [<arguments>] <string literal>: spell checks the text passed
 - spchk [<arguments>] -f <file-name>: spell check will check the indicated file

ARGs:
 - g: exit function after first mistake
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include "spell_check.h"

// Maximum length of the checked string
#define MAX_STRING_SIZE 512


int main(int argc, char *argv[]) {
    char *mistake;

    if (argc == 1) { // Nothing passed with the function
        char text[MAX_STRING_SIZE];

        printf("Please enter text to be checked: ");
        if (fgets(text, MAX_STRING_SIZE, stdin) == NULL) {
            fprintf(stderr, "spchk: Bad File Number\n");
            errno = 9;
            return 1;
        }

        mistake = spell_check(text);
    }
    else if (argc == 2) { // No arguments, just plain text
        char *text = argv[1];

        mistake = spell_check(text);
    }
    else { // Some arguments / flags specified 
        for (int i = 0; i < argc; ++i) {
            if (strcmp(argv[i], "-f") == 0) {
                if (i + 1 < argc) {
                    int fd = open(argv[i + 1], O_RDONLY);
                    int r = 0;
                    char buf[512];

                    if (fd < 0) {
                        fprintf(stderr, "spchk: Process exited error code: %d\n", errno);
                        return 1;
                    }

                    while ((r = read(fd, buf, 512)) > 0) {
                        mistake = spell_check(buf);
                        if (mistake != NULL) break;
                    }

                    close(fd);

                    if (r < 0) {
                        fprintf(stderr, "spchk: Process exited error code: %d\n", errno);
                        return 1;
                    }

                    ++i;
                }
                else {
                    fprintf(stderr, "spchk: command syntax error near -f\n");
                    return 1;
                }
            }
        }
    }

    if (mistake == NULL) {
        printf("No spelling errors found!\n");        
    }
    else {
        printf("Spelling error found near: %s\n", mistake);
    }
}