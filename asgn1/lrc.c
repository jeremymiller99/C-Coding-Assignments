#include <stdio.h>
#include <stdlib.h>
#include "names.h"

typedef enum {DOT, LEFT, CENTER, RIGHT} Position;

int getPlayerCount();
void setupChips(int num_players, int chips[]);
void playerTurn(int currentIndex, int num_players, int chips[], char roll);
int isGameOver(int num_players, int chips[]);
char rollDice();
void getSeed();

int main()
{       
	int player_count = getPlayerCount();
	printf("Number of Players: %d\n", player_count);	
	
	getSeed();

	int chips[player_count];	
	setupChips(player_count, chips);
	
	while(isGameOver(player_count, chips) < 1){
		for (int i = 0; i < player_count; i++) {
			if (chips[i] > 0){
				char roll = rollDice();
            			playerTurn(i, player_count, chips, roll);
				printf("%s: ", player_name[i]);
				printf(" ends her turn with %d\n", chips[i]);
			}
		}
	}	
	for (int i = 0; i < player_count; i++) {
                if (chips[i] > 0) {
                	printf("%s won!\n", player_name[i]);
                }
        }	

}

void setupChips(int num_players, int chips[]){
        for (int i = 0; i < num_players; i++) {
                chips[i] = 3;
       }
}

char rollDice(){
	const Position die[6] = {
                DOT,
                DOT,
                DOT,
                LEFT,
                CENTER,
                RIGHT
        };

	return die[random() % 6];
}

void playerTurn(int currentIndex, int num_players, int chips[], char roll){
	if(chips[currentIndex] > 0){
		if(roll == LEFT){
			chips[currentIndex]--;
			chips[(currentIndex - 1 + num_players) % num_players]++;
		} 
       		if(roll == RIGHT){
			chips[currentIndex]--;
                	chips[(currentIndex + 1) % num_players]++;
		}
		if(roll == CENTER){
			chips[currentIndex]--;
        	}
		if(roll == DOT){
                	//pass
        	}
	}
}

int getPlayerCount(){
	int num_players = 3;
        printf("Number of players (3 to 10)? ");
        int scanf_result = scanf("%d", &num_players);
	
	if (scanf_result < 1 || num_players < 3 || num_players > 10) {
                printf("Invalid number of players. Using 3 instead.\n");
                return 3;
        } else {
		return num_players;
        }
}

void getSeed(){
        unsigned seed;
	printf("Random-number seed? ");
        int scanf_result = scanf("%u", &seed);
        if (scanf_result < 1){
                printf("Invalid seed. using 4823 instead.\n");
		srandom(4823);
	} 
	srandom(seed);
}

int isGameOver(int num_players, int chips[]){
	int activePlayers = 0;
	for (int i = 0; i < num_players; i++) {
        	if (chips[i] > 0) {
            	activePlayers++;
            		if (activePlayers > 1) {
                		return 0;
            		}		
        	}
    	}
	
	return activePlayers == 1;
}
