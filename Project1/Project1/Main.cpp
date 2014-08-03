#include "Matrix.cpp"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv){


/*
	Matrix2 m2 = Matrix2(2, 2);*/
	Matrix<float>* m1 = new Matrix<float>(2,2);
	Matrix<float>* m2 = new Matrix<float>(2,2);

	m1->randomize();
	m2->randomize();

	Matrix<float>* r = Matrix<float>::multiply(m1, m2);


	m1->print();
	printf("\n");
	m2->print();
	printf("\n");
	r->print();

	r = r->inverse();

	printf("\n");
	r->print();
	
	delete m1;
	delete m2;
	delete r;

	while (1);
	return 1;
}