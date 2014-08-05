#include "Matrix.cpp"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv){

	auto m1 = Matrix<float>(2,2);	
	Matrix<float> out;
	
	m1.randomize();

	m1.print();
	printf("\n");

	float det;
	//Matrix<float>::upperTriangulate(&m1, &out, det);
	Matrix<float>::inverse(&m1, &out, det);

	printf("%f\n\n", det);

	m1.print();
	printf("\n");

	out.print();
	printf("\n");

	while (1);
	return 1;
}