#include <stdio.h>
#include <stdlib.h>

void matrix_output(int height, int width, int ** matrix){
	for (int i=0; i<height; i++){
		for (int j=0; j<width; j++){
			printf("%d ", matrix[i][j]);
		}
	printf("\n");
	}
	printf("\n");
	return;
}

int ** matrix_input(int height, int width){
 	int ** matrix = (int **) malloc(height * sizeof(int *));
 	for (int i=0; i < height; i++){
 		matrix[i] = (int *) malloc(width * sizeof(int));
 	}
	for (int i=0; i<height; i++){
		for (int j=0; j<width; j++){
			scanf("%d", &(matrix[i][j]));
		}
	}
	printf("\n");
	matrix_output(height, width, matrix);
	return matrix;
}

int ** add(int h, int w, int ** m1, int ** m2){
	int ** res = (int**) malloc(h*sizeof(int*));
	for (int i=0; i<h; i++){
		res[i] = (int *) malloc(w*sizeof(int));
	}
	for (int i=0; i<h; i++){
		for (int j=0; j<w; j++){
			res[i][j] = m1[i][j] + m2[i][j];
		}
	}
	return res;
}

int ** matrix_mul(int h1, int w1, int ** m1, int h2, int w2, int ** m2){
	int ** res = (int**) malloc(h1*sizeof(int*));
	for (int i=0; i<h1; i++){
		res[i] = (int *) malloc(w2*sizeof(int));
	}
	for (int i=0; i<h1; i++){
		for (int j=0; j<w2; j++){
			res[i][j] = 0;
			for (int k=0; k<w1; k++){
				res[i][j] += m1[i][k] * m2[k][j];
			}
		}
	}
	return res;
}

int ** num_mul(int alpha, int h, int w, int ** matrix){
	int ** res = (int**) malloc(h*sizeof(int*));
	for (int i=0; i<h; i++){
		res[i] = (int *) malloc(w*sizeof(int));
	}
	for (int i=0; i<h; i++){
		for (int j=0; j<w; j++){
			res[i][j] = matrix[i][j];
			res[i][j] *= alpha;
		}
	}
	return res;
}

void free_matrix(int h, int w, int *** matrix){

	for (int i=0; i<h; i++){
		free((*matrix)[i]);
	}
	free(*matrix);
	*matrix = NULL;
	return;
}


int** minor(int column, int width, int ** matrix){
	int ** res = (int**) malloc((width-1)*sizeof(int*));
	for (int i=0; i<(width-1); i++){
		res[i] = (int *) malloc((width-1)*sizeof(int));
	}
	int k = 0;
	for (int i=1; i < width; i++){
		for (int j=0; j<width; j++){
			if (j > column)
				k = j - 1;
			else
				k = j;
			res[i-1][k] = matrix[i][j];
		}
	}
	return res;
}

float determinant(int width, int ** matrix){
	float res = 0.0;
	if (width == 1)
		return matrix[0][0];
	if (width == 2){
		res = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
		return res;
	}
	int sigma = 1;
	for (int j=0; j<width; j++){
		int ** min = minor(j, width, matrix);
		res += sigma * matrix[0][j] * determinant(width - 1, min);
		free_matrix(width-1, width-1, &min);
		sigma *= -1;
	}
	return res;
}
