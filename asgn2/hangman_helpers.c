#include "hangman_helpers.h"

bool is_lowercase_letter(char c) {
	if (c >= 'a' && c <= 'z') {
		return true;
    	} else {
		return false;
    }
}

bool validate_secret(const char *secret){ 
	 
}

bool string_contains_character(const char *s, char c) {
	while(*s != '\0') {
		if(*s == c) {
			return true;
		}
		s++;
	}
	return false;
}

char read_letter(void) {
	printf("Enter your guess (single letter): ");
        char guess;
        scanf(" %c", &guess);
	return guess;
}
