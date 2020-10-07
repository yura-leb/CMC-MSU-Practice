#include <stdio.h>
#include "Task2.h"

/*Для работы с матрицами введены:

Функции, возвращающие новую матрицу:

1)MatrixInput(int height, int width) 
Ввод матрицы с клавиатуры через enter с заданными высотой height и шириной width.
Матрица сразу выведется на экран, чтобы было нагляднее.
В случае неверно введённых данных, начиная с неправильно введённого числа всё заполняется нулями.


***ВАЖНО***
Для каждой функции матрица, в которую присваивается значение должна иметь значение NULL.

2)AddMatrices(int height, int width, int **matrix1, int **matrix2)
Сумма двух матриц с одинаковыми размерами.

3)MultiplyMatrices(int height1, int width1, int **matrix1, int height2, int width2, int **matrix2)
Перемножение матриц, где ширина первой должна быть равно высоте второй матрицы.

4)MultiplyNumberOnMatrix(int alpha, int height, int width, int **matrix)
Умножение матрицы на число alpha.

5)MinorMatrix(int column, int width, int **matrix)
вспомогательная функция для вычисления определителя,
которая возвращает минорную матрицу без верхней строки и столбца с номером column.
Минор вычисляется только от квадратной матрицы.


Функция Determinant(int width, int **matrix)
Возвращает значение определителя матрицы типа float.
Работает только с квадратной матрицей.

Функция MatrixOutput(int height, int width, int **matrix)
выводит матрицу

По завершении работы с программой необходимо вызвать функцию
FreeMatrix(int height, int width, int ***matrixPointer) для каждой использованной матрицы
она очистит память, использованную для работы с матрицами

Пример работы программы: */

int main(int argc, char const *argv[]){
	int height = 2;
	int width = 3;
	int ** matrix1 = MatrixInput(height, width);
	int ** matrix2 = MatrixInput(height, width);
	int ** matrix3 = AddMatrices(height, width, matrix1, matrix2);
	MatrixOutput(height, width, matrix3);
	FreeMatrix(height, width, &matrix1);
	FreeMatrix(height, width, &matrix2);
	FreeMatrix(height, width, &matrix3);
	return 0;
}



