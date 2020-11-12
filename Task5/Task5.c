#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

char **divide_string_in_words(char *string) {
	int words_count = 8;
	int size_of_word = 16;
	int length_of_word = 16;
	char **words = (char **) malloc(sizeof(char *) * words_count);
	int i = 0; //номер символа в строке
	int j = 0; //номер нового слова
	int k = 0; //номер символа в новом слове
	char *word = (char *)malloc(sizeof(char) * size_of_word);
	int not_end_of_string = (string[i] != '\0');
	while (1){
		int is_bracket = ((string[i] == '(') || (string[i] == ')'));
		int is_sign = (string[i] == '&') || (string[i] == '|') || (string[i] == '>') || (string[i] == '<');
		int is_space = (string[i] == ' ');
		int is_end_of_word = is_space || (string[i] == '\0') || is_bracket || is_sign || (string[i] == ';');
		if (is_end_of_word) {
			int is_prev_space = (string[i-1] == ' ');
			if (is_prev_space && (string[i] == ' ')) {
				i++;
			} else {
				if (j >= words_count - 1) {
					words_count += words_count;
					words = (char **)realloc(words, sizeof(char *) * words_count);
				}
				if (word[0] != '\0') {
					word[k] = '\0';
					words[j] = (char *) malloc(sizeof(char) * (strlen(word) + 1));
					strcpy(words[j], word);
					free(word);
					word = malloc(sizeof(char) * size_of_word);
					word[0] = '\0';
					j++;
				}
				int is_end_of_string = (string[i] == '\0');
				if (is_end_of_string)
					break;
				if (is_bracket || (string[i] == ';')) {
					words[j] = (char *) malloc(sizeof(char) * (strlen(word) + 1));
					word[0] = string[i];
					word[1] = '\0';
					strcpy(words[j], word); 
					free(word);
					word = malloc(sizeof(char) * size_of_word);
					word[0] = '\0';
					j++;
				}
				if (is_sign) {
					if (string[i + 1] && (string[i + 1] == string[i])) {
						words[j] = (char *) malloc(sizeof(char) * (strlen(word) + 1));
						word[0] = string[i];
						word[1] = string[i + 1];
						word[2] = '\0';
						strcpy(words[j], word); 
						free(word);
						word = malloc(sizeof(char) * size_of_word);
						word[0] = '\0';
						j++;
						i++;
					} else {
						words[j] = (char *) malloc(sizeof(char) * (strlen(word) + 1));
						word[0] = string[i];
						word[1] = '\0';
						strcpy(words[j], word); 
						free(word);
						word = malloc(sizeof(char) * size_of_word);
						word[0] = '\0';
						j++;
					}
				}
				i++;
				k = 0;
			}
		} else {
			if (string[i] == '"') {
				i++;
				while (string[i] != '"') {
					if (k >= length_of_word) {
						length_of_word += size_of_word;
						word = realloc(word, length_of_word * sizeof(char));
					}
					word[k] = string[i];
					i++;
					k++;
				}
				i++;
			} else {
				if (k >= length_of_word) {
					length_of_word += size_of_word;
					word = realloc(word, length_of_word * sizeof(char));
				}
				word[k] = string[i];
				i++;
				k++;
			}
		}
		not_end_of_string = (string[i] != '\0');
	}
	free(word);
	return words;
}

char **get_string(FILE *input_file) {
	int const step = 16;
	int const words_count = 8;
	char **words = NULL;
	int size = step, length, position = 0;
	char *string = (char *) malloc(step * sizeof(char));
	int is_string_exists = (fgets(string+position, step, input_file) != NULL);
	is_string_exists = is_string_exists && (string[0] != '\n');
	if (!(is_string_exists)) {
		free(words);
		words = NULL;
	}
	while (is_string_exists){
		length = strlen(string);
		if (string[length-1] != '\n'){
			position = length;
			size += step;
			string = realloc(string, size);
		} else {
			string[length-1] = '\0';
			words = divide_string_in_words(string);
			return words;
		}
		is_string_exists = (NULL != fgets(string+position, step, input_file));
	}
	free(string);
	return NULL;
}

int change_directory(char **words) {
	int is_nothing_after_cd = !(words[1]);
	if (is_nothing_after_cd) {
		if (chdir(getenv("HOME")) == 0) {
			printf("Got home\n");
			return 0;
		}
	}
	int is_only_one_parameter = !(words[2]);
	if (is_only_one_parameter) {
		if (chdir(words[1]) == 0) {
			printf("Directory changed\n");
			return 0;
		}
	}
	//Если неправильное количество параметров
	return 1;
}

int do_command(char **words) {
	int is_first_cd = (strcmp(words[0], "cd") == 0);
	if (is_first_cd) {
		if (change_directory(words) == 0){
		} else {
			printf("Change directory error\n");
		}
	} else {	
		if (fork() == 0) {
			char *path = words[0];
			execvp(path, words);
			printf("Execution error\n");
			exit(1);
		}
		wait(NULL);
	}
	return 0;
}

void do_all(FILE *input_file){
	char **words;
	char **is_input_exists = (words = get_string(input_file));
	if (!(words)) {
		printf("No input\n");
		return;
	}
	while (1){
		if (is_input_exists) {
			int length = 0;
			while (words[length])
				length++;
			do_command(words);
			for (int i = 0; i < length; i++) {
				free(words[i]);
				words[i] = NULL;
			}
			free(words);
			words = NULL;
			is_input_exists = (words = get_string(input_file));
		} else {
			break;
		}
	}
	free(words);
	return;
}

int main(int argc, char const *argv[])
{
	FILE *input_file;
	if (argc == 2) {
		char const *filename = argv[1];
		if (input_file = fopen(filename, "r")) {
			do_all(input_file);	
		} else {
			printf("%s: this file doesn't exist\n", filename);
			return 1;
		}
	} else {
		do_all(stdin);
	}
	
	return 0;
}