void MatrixOutput(int height, int width, int ** matrix);
int **MatrixInput(int height, int width);
int **AddMatrices(int height, int width, int **matrix1, int **matrix2);
int **MultiplyMatrices(int height1, int width1, int **matrix1, int height2, int width2, int **matrix2);
int **MultiplyNumberOnMatrix(int alpha, int height, int width, int **matrix);
void FreeMatrix(int height, int width, int ***matrixPointer);
float Determinant(int width, int **matrix);

