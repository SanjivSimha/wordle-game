#include "wordle_lib.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

bool score_guess(char *secret, char *guess, char *result) {

  bool secret_letters[5] = {false};
  bool guess_letters[5] = {false};

  for (int i = 0; i < 5; i++){
	  if (guess[i] == secret[i]){
		  result[i] = 'g';
		  secret_letters[i] = true;
		  guess_letters[i] = true;
	  } else {
		  result[i] = 'x';
	  }
  }
  for (int i = 0; i < 5; i++){
	  if (result[i] == 'x'){
		  for (int j = 0; j < 5; j++){
			  if (!secret_letters[j] && guess[i] == secret[j]){
				  result[i] = 'y';
				  secret_letters[j] = true;
				  break;
			  }
		  }
	  }
  }
  result[5] = '\0';
  return strcmp(secret, guess) == 0;
}

bool valid_guess(char *guess, char **vocabulary, size_t num_words) {
  for (size_t i = 0; i < num_words; i++){
	  if (strcmp(guess, vocabulary[i]) == 0){
		  return true;
	  }
  }
  return false;
}

char **load_vocabulary(char *filename, size_t *num_words) {
  char **pointer = NULL;
  size_t size = 10;
  *num_words = 0;
  FILE *file = fopen(filename, "r");
  if (!file) {
	  perror("Error opening file");
	  return NULL;
  }
  pointer = malloc(size * sizeof(char*));
  if (!pointer){
	  perror("Error allocating memory");
	  fclose(file);
	  return NULL;
  }
  char buffer[6];
  while (fgets(buffer, sizeof(buffer), file)){
	  buffer[strcspn(buffer, "\n")] = '\0';
	  char *word = strdup(buffer);
	  if (!word){
		  perror("Error duplicating word");
		  break;
	  }
	  pointer[*num_words] = word;
	  (*num_words)++;
	  if (*num_words >= size){
		  size *= 2;
		  char **new_pointer = realloc(pointer, size* sizeof(char *));
		  if (!new_pointer){
			  perror("Error reallocating memory");
			  break;
		  }
		  pointer = new_pointer;
	  }
  }
  fclose(file);
  return pointer;
}

void free_vocabulary(char **vocabulary, size_t num_words) {
  for (size_t i = 0; i < num_words; i++){
	  free(vocabulary[i]);
  }
  free(vocabulary);
}
