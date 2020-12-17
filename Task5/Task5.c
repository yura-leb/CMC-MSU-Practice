#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

void is_bracket_or_semicolon(char **words, char *word, char *string, int *j, int *i) {
	int size_of_word = 16;
	words[*j] = (char *) malloc(sizeof(char) * (strlen(word) + 1));
	word[0] = string[*i];
	word[1] = '\0';
	strcpy(words[*j], word); 
	free(word);
	word = malloc(sizeof(char) * (size_of_word + 1));
	word[0] = '\0';
	(*j)++;
	return;
}

void is_sign_function(char **words, char *word, char *string, int *j, int *i) {
	int size_of_word = 16;
	if (string[(*i) + 1] && (string[(*i) + 1] == string[*i])) {
		words[*j] = (char *) malloc(sizeof(char) * 4);
		word[0] = string[*i];
		word[1] = string[(*i) + 1];
		word[2] = '\0';
		strcpy(words[*j], word); 
		free(word);
		word = malloc(sizeof(char) * (size_of_word + 1));
		word[0] = '\0';
		(*j)++;
		(*i)++;
	} else {
		words[*j] = (char *) malloc(sizeof(char) * 3);
		word[0] = string[*i];
		word[1] = '\0';
		strcpy(words[*j], word); 
		free(word);
		word = malloc(sizeof(char) * (size_of_word + 1));
		word[0] = '\0';
		(*j)++;
	}
}


char **divide_string_in_words(char *string) {
	int words_count = 8;
	int size_of_word = 16;
	int length_of_word = 16;
	char **words = (char **) malloc(sizeof(char *) * (words_count + 1));
	int i = 0;
	int j = 0;
	int k = 0;
	char *word = (char *)malloc(sizeof(char) * (size_of_word + 1));
	int not_end_of_string = (string[i] != '\0');
	char space = ' ';
	while (1){
		int is_bracket = ((string[i] == '(') || (string[i] == ')'));
		int is_sign = (string[i] == '&') || (string[i] == '|') || (string[i] == '>') || (string[i] == '<');
		int is_space = (string[i] == space);
		int is_end_of_word = is_space || (string[i] == '\0') || is_bracket || is_sign || (string[i] == ';');
		if (is_end_of_word) {
			int is_prev_space = (string[i-1] == space);
			if (is_prev_space && (string[i] == space)) {
				i++;
			} else {
				if (j >= words_count - 1) {
					words_count += words_count;
					words = (char **)realloc(words, sizeof(char *) * (words_count + 1));
				}
				if (word[0] != '\0') {
					word[k] = '\0';
					words[j] = (char *) malloc(sizeof(char) * (strlen(word) + 1));
					strcpy(words[j], word);
					free(word);
					word = malloc(sizeof(char) * (size_of_word + 1));
					word[0] = '\0';
					j++;
				}
				int is_end_of_string = (string[i] == '\0');
				if (is_end_of_string)
					break;
				if (is_bracket || (string[i] == ';')) {
					is_bracket_or_semicolon(words, word, string, &j, &i);
				}
				if (is_sign) {
					is_sign_function(words, word, string, &j, &i);
				}
				i++;
				k = 0;
			}
		} else {
			if (string[i] == '"') {
				i++;
				while (string[i] != '"') {
					if (k >= length_of_word) {
						length_of_word += (size_of_word + 1);
						word = realloc(word, (length_of_word + 1) * sizeof(char));
					}
					word[k] = string[i];
					i++;
					k++;
				}
				i++;
			} else {
				if (k >= length_of_word) {
					length_of_word += size_of_word + 1;
					word = realloc(word, (length_of_word + 1) * sizeof(char));
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
			free(string);
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

int conveyor(char **parameters) {
	int length = 0, file_descriptor[2], process_id, status;
	while (parameters[length])
  		length++;
  	char **process = malloc(length * sizeof(char *));
  	int k = 0;
  	int j = 0;
  	while (j < length) {
    	while ((j < length) 
	    	&& (strcmp(parameters[j], "|") != 0) 
	    	&& (strcmp(parameters[j], "||") != 0)
	    	&& (strcmp(parameters[j], "&") != 0)
			&& (strcmp(parameters[j], "&&") != 0)) {
	    	process[k] = parameters[j];
	    	k++;
	    	j++;
	    }
    	if (process[k])
    		process[k] = NULL;
    	int one_process = ((j >= length) || (strcmp(parameters[j], "|") == 0));
	    if (one_process) {
			pipe(file_descriptor);
			switch (process_id = fork()) {
				case -1:
	        		printf("Fork conveyor error\n");
	        		exit(-1);
	        	case 0:
	          		if (j < (length - 1))
	            		dup2(file_descriptor[1], 1);
					        close(file_descriptor[0]);
					        close(file_descriptor[1]);
					        execvp(process[0], process);
					        printf("Execvp conveyor error\n");
					        exit(-1);
			}
	    	dup2(file_descriptor[0], 0);
	    	close(file_descriptor[0]);
	      	close(file_descriptor[1]);
	      	for (int i = 0; i < k; i++) {
	        	process[i] = NULL;
	      	}
	      	k = 0;
	      	j++;
	    }
	    else if (strcmp(parameters[j], "||") == 0) {
	      	switch (process_id = fork()) {
	        	case -1:
	          		printf("Fork || error\n");
	         	 	exit(-1);
	       	 	case 0:
	          		execvp(process[0], process);
	          		printf("Execvp || error\n");
	          		exit(-1);
	      	}
	      	if (process_id = wait(&status)) {
	        	while ((j < length) && (strcmp(parameters[j], "|") != 0))
	          		j++;
	      	}
	      	for (int i = 0; i < k; i++) {
	        	process[i] = NULL;
	      	}
	      	k = 0;
	      	j++;
	    }
	    else if (strcmp(parameters[j], "&") == 0) {
	      	switch (process_id = fork()){
	        	case -1:
	          	printf("Fork error\n");
	          	exit(-1);
	        	case 0:
	          	{
	            	switch (process_id = fork()){
	              		case -1:
	                		printf("Fork error\n");
	                		exit(-1);
	              		case 0:
	              		{
	                		signal(SIGINT, SIG_IGN);
	                		int file = open("/dev/null", O_RDWR, 0644);
	                		dup2(file, 0);
	                		//dup2(file, 1);
	                		execvp(process[0], process);
	                		printf("Background error");
	                		exit(-1);
	              		}
	            	}
	            	exit(0);
	        	}
	      	}
	      	for (int i = 0; i < k; i++) {
	        	process[i] = NULL;
	      	}
	      	k = 0;
	      	j++;
	    }
	    else if (strcmp(parameters[j], "&&") == 0) {
	      	switch (process_id = fork()) {
	        	case -1:
	          		printf("Fork && error\n");
	          		exit(-1);
	        	case 0:
	          		execvp(process[0], process);
	          		printf("Execvp && error\n");
	          		exit(-1);
	      	}
	      	if (!(process_id = wait(&status))) {
	        	while ((j < length) && (strcmp(parameters[j], "|") != 0))
	          		j++;
	      	}
	      	for (int i = 0; i < k; i++) {
	        	process[i] = NULL;
			}
			k = 0;
			j++;
	    }
	}
  	while ((process_id = wait(NULL)) != -1)
    	printf("Process %d finished\n", process_id);
  	free(process);
  	return 0;
}

int do_command(char **words) {
	int is_first_cd = (strcmp(words[0], "cd") == 0);
	if (is_first_cd) {
		if (change_directory(words) == 0){
		} else {
			printf("Change directory error\n");
		}
	} else {
		int i = 0, k = 0;
		int words_count = 0;
		while (words[words_count])
			words_count++;
		char **parameters = malloc((words_count + 1) * sizeof(char *));
		i = 0;
		int file_descriptor[2];
		int process_id;
		switch (process_id = fork()) {
			case -1:
				printf("Fork error\n");
				exit(1);
			case 0:
				while (i <= words_count) {
					int is_not_end_of_conveyor = (i < words_count) && (strcmp(words[i], ";") != 0);
					if (is_not_end_of_conveyor) {
						if (strcmp(words[i], ">") == 0) {
							int file = open(words[i + 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
							if (file == -1) {
								printf("File error\n");
								exit(1);
							}
							i++;
							dup2(file, 1);
							close(file);
						} else if (strcmp(words[i], ">>") == 0) {
							int file = open(words[i + 1], O_WRONLY | O_APPEND | O_CREAT, 0644);
							if (file == -1) {
								printf("File error\n");
								exit(1);
							}
							dup2(file, 1);
							close(file);
							i++;
						} else if (strcmp(words[i], "<") == 0) {
							int file = open(words[i + 1], O_RDONLY, 0644);
							if (file == -1) {
								printf("File error\n");
								exit(1);
							}
							dup2(file, 0);
							close(file);
							i++;
						} else 
							parameters[k] = words[i];
						i++;
						k++;
					} else {
						if (parameters[k])
							parameters[k] = NULL;
						conveyor(parameters);
						for (int j = 0; j < k + 1; j++) {
							parameters[j] = NULL;
						}
						k = 0;
						i++;
					}
				}
				free(parameters);
				exit(1);
		}
		wait(NULL);
	}
	return 0;
}

void do_all(FILE *input_file){
	char **words = NULL;
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
		char const *file_name = argv[1];
		if (input_file = fopen(file_name, "r")) {
			do_all(input_file);	
		} else {
			printf("%s: this file doesn't exist\n", file_name);
			return 1;
		}
	} else {
		do_all(stdin);
	}
	
	return 0;
}
