#include "Matrix.cpp"
#include "Vector.cpp"


#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv){
    

	auto m1 = Matrix<float>(3,3);	
    Matrix<float> out;
	
    Vector<float> v1 = m1.getColumnView(1);
	m1.randomize();


    printf("before\n");
	m1.print();
    
	float det;
	//Matrix<float>::upperTriangulate(&m1, &out, det);
    Matrix<float>::inverse(m1, out, det);
    
	printf("%f\n\n", det);
    
	m1.print();
	printf("\n vectot\n");
    v1.print();



	while (1);
	return 1;
}