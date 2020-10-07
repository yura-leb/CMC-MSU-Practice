#include <stdio.h>
#include <stdlib.h>

//Вывод матрицы на экран.
void MatrixOutput(int height, int width, int **matrix) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j<width; j++) {
			printf("%d ", matrix[i][j]);
		}
	printf("\n");
	}
	printf("\n");
	return;
}

//Ввод матрицы: каждый элемент вводится в новой строке.
int **MatrixInput(int height, int width) {
 	int **matrix = (int **) malloc(height * sizeof(int *));
 	for (int i = 0; i < height; i++) {
 		matrix[i] = (int *) malloc(width * sizeof(int));
 	}
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			scanf("%d", &(matrix[i][j]));
		}
	}
	printf("\n");
	MatrixOutput(height, width, matrix);
	return matrix;
}

//Сложение матриц одинакового размера.
int **AddMatrices(int height, int width, int **matrix1, int **matrix2) {
	int ** result = (int **) malloc(height * sizeof(int *));
	for (int i = 0; i < height; i++)
		result[i] = (int *) malloc(width * sizeof(int));
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			result[i][j] = matrix1[i][j] + matrix2[i][j];
		}
	}
	return result;
}

//Перемножение матриц. Ширина первой должна быть равна высоте второй.
int **MultiplyMatrices(int height1, int width1, int **matrix1, int height2, int width2, int **matrix2) {
	int **result = (int **) malloc(height1 * sizeof(int *));
	for (int i = 0; i < height1; i++) {
		result[i] = (int *) malloc(width2 * sizeof(int));
	}
	for (int i = 0; i < height1; i++) {
		for (int j = 0; j < width2; j++) {
			result[i][j] = 0;
			for (int k = 0; k < width1; k++) {
				result[i][j] += matrix1[i][k] * matrix2[k][j];
			}
		}
	}
	return result;
}

//Умножение матрицы на число.
int **MultiplyNumberOnMatrix(int alpha, int height, int width, int **matrix) {
	int **result = (int **) malloc(height * sizeof(int *));
	for (int i = 0; i < height; i++) {
		result[i] = (int *) malloc(width * sizeof(int));
	}
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			result[i][j] = matrix[i][j];
			result[i][j] *= alpha;
		}
	}
	return result;
}

//Освобождение памяти, использованной для хранения матрицы.
void FreeMatrix(int height, int width, int ***matrixPointer) {
	for (int i = 0; i < height; i++) {
		free((*matrixPointer)[i]);
	}
	free(*matrixPointer);
	*matrixPointer = NULL;
	return;
}

/*Поиск минорной матрицы, то есть матрицы без верхней строки и конкретного столбца.
Нужно для вычисления определителя матрицы.*/
int **MinorMatrix(int column, int width, int **matrix) {
	int **result = (int **) malloc((width-1) * sizeof(int *));
	for (int i = 0; i < (width-1); i++) {
		result[i] = (int *) malloc((width-1) * sizeof(int));
	}
	int k = 0;
	for (int i = 1; i < width; i++) {
		for (int j = 0; j < width; j++) {
			if (j > column)
				k = j - 1;
			else
				k = j;
			result[i-1][k] = matrix[i][j];
		}
	}
	return result;
}
//Вычисление определителя квадратной матрицы 
float Determinant(int width, int **matrix) {
	float result = 0.0;
	int sigma = 1;
	switch (width) {
		case 1:
			return matrix[0][0];
			break;
		case 2:
			result = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
			return result;
			break;
		default:
			for (int j = 0; j < width; j++) {
				int **minor = MinorMatrix(j, width, matrix);
				result += sigma * matrix[0][j] * Determinant(width - 1, minor);
				FreeMatrix(width-1, width-1, &minor);
				sigma *= -1;
			}
			return result;
	}
}

