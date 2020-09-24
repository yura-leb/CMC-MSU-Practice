#include <stdio.h>

int del(int a){
	int k = 0;
	int b;
	for (long i = 0; i < 4 * sizeof(int); i++){
		b = a & (1 << i);
		if (b) k++;
		if (k % 2 == 0) a = a & ~(1 << i);
	}
	return a;
}

int main(int argc, char const *argv[])
{
	int a;
	scanf("%d\n", &a);
	printf("%d\n", del(a));
	return 0;
}