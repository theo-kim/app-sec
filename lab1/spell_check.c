#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "spell_check.h"

// This function reviews the words in text (tokenized by "whitespace") and 
// determines if they were spelt right by comparing them to the build in words
// in Ubuntu /usr/share/dict/words
// :text: pointer to str of input
// returns pointer to first mispelt word found.
char *spell_check(char *text) {
    // Open dictionary
    int dictfd = open("/usr/share/dict/words", O_RDONLY);
    if (dictfd < 0) {
        return NULL;
    }
    FILE *f = fdopen(dictfd, "r");

    int cap = 1;

    // tokenize input by whitespace
    char *delimeters = " \n\t";
    char * pch = strtok(text, delimeters);
    while (pch != NULL) {
        if (cap == 1) {
            pch[0] ^= 0x20;
        }
        
        if (strchr(pch, '.') != NULL || strchr(pch, '?') != NULL || strchr(pch, '!') != NULL) {
            cap = 1;
        }
        else {
            cap = 0;
        }

        if ((pch[strlen(pch) - 1] > 91 && pch[strlen(pch) - 1] < 97) || 
            (pch[strlen(pch) - 1] > 122) || (pch[strlen(pch) - 1] < 65)) {
                pch[strlen(pch) - 1] = '\0';
            }

        if (check_word(pch, dictfd) != 0) {
            close(dictfd);
            return pch;
        }
        pch = strtok(NULL, delimeters);
        rewind(f);
    }

    close(dictfd);
    
    return NULL;
}

// This function reviews the word for correct spelling (according to
// a dictionary)
// :word: Word whose spelling is to be checked
// :dict: A reference dictionary
// returns -1 f incorrect or 0 if spelling was correct
int check_word(char *word, int dictfd) {
    char buf[512];
    int r = 0;

    while ((r = read(dictfd, buf, 512)) > 0) {
        for (int i = 0; i < 512; ++i) {
            for (int j = 0; j < strlen(word); ++j) {
                if (word[j] != buf[i] && ((j > 0) || (word[j] ^ 0x20) != buf[i])) {
                    break;
                } 
                else if (j + 1 == strlen(word) && buf[i + 1] == '\n') {
                    return 0;
                }
                else {
                    ++i;
                }

            }
        }
    }

    if (r < 0) {
        return -1;
    }
    
    return -1;
}