#include "Matrix.cpp"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv){


/*
	Matrix2 m2 = Matrix2(2, 2);*/
	Matrix<float> m1 = Matrix<float>(3,3);
	//Matrix<float> m2 = Matrix<float>(2,2);

	m1.randomize();
	//m2.randomize();

	//Matrix<float> r = m1 ^ -1;

	printf("%f\n\n", m1.determinant());

	m1.print();
	printf("\n");
	/*
	m2.print();
	printf("\n");*/
	//r.print();

	//r = r ^ -1;

	//printf("\n");
	//r.print();
	
	//delete m1;
	//delete m2;
	//delete r;

	while (1);
	return 1;
}