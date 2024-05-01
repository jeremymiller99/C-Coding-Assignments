#include "hangman_helpers.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

const char *getArt(size_t index){
        size_t art = sizeof(arts) / sizeof(arts[0]);
        if (index < art) {
                return arts[index];
        } else {
                return NULL;
        }
}

void printGameState(const char *art, const char *phrase, const char *guesses){
	printf("%s", art);
	printf("\n\n");
	printf("    Phrase: %s\n", phrase);
	printf("Eliminated: %s\n", guesses);
}

int addGuess(char* guessedLetters, char guess, int* guessedCount) {
	guess = tolower(guess);
    
	if (strchr(guessedLetters, guess)) {
        	return 0;
    	}	
    
    	guessedLetters[*guessedCount] = guess;
    	(*guessedCount)++;
    
    	return 1;
}

int main(int argc, char *argv[])
{		
	if (argc < 2) {
        	printf("Usage: %s <secret_phrase>\n", argv[0]);
		return 1;
    	}

	char secretPhrase[MAX_LENGTH];
    	strncpy(secretPhrase, argv[1], sizeof(secretPhrase) - 1);
	secretPhrase[sizeof(secretPhrase) - 1] = '\0';

    	char phrase[strlen(secretPhrase)];

    	char guesses[27] = {0};
    	size_t mistakeCount = 0;
	int total_guesses = 0;

	for (int i = 0; secretPhrase[i] != '\0'; i++) {
        	phrase[i] = '_';
        	if (secretPhrase[i] == ' '){
		       phrase[i] = ' ';
		}
    	}
    	
	printf(CLEAR_SCREEN);

    	while (mistakeCount < LOSING_MISTAKE){
			
		char guess = read_letter();
		const char *art = getArt(mistakeCount);
		
		for (int i = 0; secretPhrase[i] != '\0'; i++) {
                       	if (secretPhrase[i] == guess) {
				phrase[i] = secretPhrase[i];
			}
		}
		phrase[strlen(secretPhrase)] = '\0';
		
		if (string_contains_character(secretPhrase, guess)){
			printGameState(art, phrase, guesses);
		} else {
			if (!string_contains_character(guesses, guess)){
				addGuess(guesses, guess, &total_guesses);
				mistakeCount++;
				art = getArt(mistakeCount);
				printGameState(art, phrase, guesses);
			} else {
				art = getArt(mistakeCount);
				printGameState(art, phrase, guesses);
			}
		}
	}

	printf("\n");	
	if(mistakeCount == LOSING_MISTAKE){
		printf("You lose! The secret phrase was: %s\n", secretPhrase);
	} else {
		printf("You win! The secret phrase was: %s\n", secretPhrase);
	}
	return 0;
}	
