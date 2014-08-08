#include "Matrix.cpp"
#include "Vector.cpp"


#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv){


    //Base<int> t = Base<int>();

	auto m1 = Matrix<int>(3,4);	
	Matrix<int> out;
	
    Vector<int> v1 = m1.getColumnView(1);
	m1.randomize();
	m1.print();
	printf("\n");
    
	int det;
	//Matrix<float>::upperTriangulate(&m1, &out, det);
	Matrix<int>::inverse(m1, out, det);
    
	printf("%f\n\n", det);
    
	m1.print();
    
	printf("\n vectot\n");

    v1.print();



	while (1);
	return 1;
}