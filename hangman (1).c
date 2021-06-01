#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <string.h>

char guessWord();
int isAlreadyGuessed(char);
int setBlanks(char);
int checkIfWon();
void printBoard();
void pickword();
void reset();
int playAgain();
int isInWord(char, char*);

char* secret = NULL;
char* blanks = NULL;
char* guessingBank = NULL;

int position;
int length;
char* board[] = {"  O", "  O\n  |", "  O\n /|", "  O\n /|\\", "  O\n /|\\\n /", "  O\n /|\\\n / \\",};

int main(int argc, char** argv) {
	puts("HANGMAN:\n");	
	do {
		reset();
		while(position < 6) {
			printBoard();
			char guess = guessWord();
			if(isAlreadyGuessed(guess)) continue;
			else if(setBlanks(guess)) {
				if(checkIfWon()) break;
			} else {
				guessingBank[position++]=guess;
			}
		} 
		if(position >= 6) printf("You lost the word was %s\n", secret);
	} while(playAgain());
	free(secret);
	free(blanks);
	free(guessingBank);
	return 0;
}

void reset() {
	position = 0;
	pickword();
}

void printBoard() {
	puts(board[position]);
	printf("\n%s:\n", blanks);
}

char guessWord() {
	puts("Enter a letter\n");
	char c = getchar();
	if(getchar() != '\n') {
		puts("Please enter only 1 letter next time\n");
		while(getchar() != '\n');
	} else if(isalpha(c)) return tolower(c);
	return guessWord();
}

int isAlreadyGuessed(char guess) {
	if(isInWord(guess, guessingBank)) {
		puts("You already guessed that letter\n");
		return 1;
	} return 0;
}

int setBlanks(char guess) {
	int boolean = 0;
	for(int i = 0; i < length; ++i) {
		if(secret[i] == guess) {
			blanks[i] = guess;
			boolean = 1;
		}
	}
	return boolean;
}

int playAgain() {
	puts("Enter y to play again\n");
	char c = getchar();
	while(getchar() != '\n');
	return tolower(c) == 'y';
}

int checkIfWon() {
	if(strcmp(secret, blanks) == 0) {
		printf("You got it man! it was %s\n", secret);
		return 1;
	} return 0;
}

int isInWord(char guess, char* word) {
	for(int i = 0; i < position; ++i) {
		if(word[i] == guess) return 1;	
	} return 0;
}

void pickword() {
	FILE* file;
	int lines = 0;
	char c = 0;

	file = fopen("words.txt", "r");

	while((c=getc(file)) != EOF) if(c == '\n') ++lines;

	fseek(file, 0, SEEK_SET);

	srand(time(NULL));

	int randomLine = 1 + rand() % lines;
	int seekWhere = 0;
	int characters = 0;

	lines = 0;
	c = 0;
	while(lines < randomLine && (c=getc(file)) != EOF) {
		if(lines+1 == randomLine) {
			++characters;
		} else {
			++seekWhere;
		}

		if(c == '\n') ++lines;
	}
	--characters;
	length = characters;
	if(secret == NULL && blanks == NULL && guessingBank == NULL) {
		secret = (char*) malloc(characters+1);
		blanks = (char*) malloc(characters+1);
		guessingBank = (char*) malloc(7);
	} else {
		secret = realloc(secret, characters+1);
		blanks = realloc(blanks, characters+1);
	}
	secret[characters] = '\0';
	blanks[characters] = '\0';
	guessingBank[6] = '\0';
	fseek(file, seekWhere, SEEK_SET);
	fread(secret, 1, characters, file);
	for(int i = 0; i < characters; ++i) {
		blanks[i] = '-';
	}

	for(int i = 0; i < 6; ++i) {
		guessingBank[i] = ' ';
	}
	fclose(file);
}
