#include "Matrix.cpp"
#include "Vector.cpp"


#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv){
    
    int size = 1024;

    //auto m = Matrix<float>(3,3);
    //Matrix<float> t;

    //m.randomize();

    //Matrix<float>::transpose(m,t);

    //m.print(t);

    for (int i = 0; i < 1; i++){
        auto m1 = Matrix<float>(size, size);
        auto m2 = Matrix<float>(size, size);
	    
        m1.randomize();
        m2.randomize();
        Matrix<float> out;
        //m1.print();
        //m2.print();
    
        Matrix<float>::multiply(m1, m2, out);
        //Matrix<float>::blockMultiply(m1, m2, out);
    }
	return 1;
}