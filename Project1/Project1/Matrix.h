#pragma once
#include "Vector.h"

template<typename T> class Matrix
{
public:
	Matrix();
	Matrix(const Matrix<T>&);
	Matrix(const int, const int);
	~Matrix();

	void initialize(int, int);
	
	static void Identity	    (const int size  , Matrix<T>&				   );
	static void Identity	    (const int x	 , const int y	   , Matrix<T>&);
	static void inverse		    (const Matrix<T>&, Matrix<T>&				   );
	static void inverse		    (Matrix<T>&		 , Matrix<T>&	   , T&		   );
	static void exponentiate    (const Matrix<T>&, const int	   , Matrix<T>&);
	static void multiply	    (const Matrix<T>&, const Matrix<T>&, Matrix<T>&);
	static void blockMultiply   (const Matrix<T>&, const Matrix<T>&, Matrix<T>&);
	static void blockMultiply2  (const Matrix<T>&, const Matrix<T>&, Matrix<T>&);
	static void add			    (const Matrix<T>&, const Matrix<T>&, Matrix<T>&);
	static void upperTriangulate(Matrix<T>&      , Matrix<T>&      , T&);
    static void transpose       (const Matrix<T>&, Matrix<T>&);

    inline T& operator()(const int&, const int&)const; 
    inline virtual T& operator()(const int&)const;

	Matrix<T> operator*(const Matrix<T>&)const;
	Matrix<T> operator+(const Matrix<T>&)const;
	Matrix<T> operator^(const int&)const;

    Matrix<T> operator*(const T&)const;
    Matrix<T> operator/(const T&)const;
    Matrix<T> operator+(const T&)const;
    Matrix<T> operator-(const T&)const;

    Matrix<T>& operator*=(const T&);
    Matrix<T>& operator/=(const T&);
    Matrix<T>& operator+=(const T&);
    Matrix<T>& operator-=(const T&);

    void scaleSubtract(const Matrix<T>&, const T&);

	T determinant() const;

	Vector<T> getColumnView(const int)const;
	Vector<T> getRowView(const int)const;

    void randomize();
    void print();
    void print(Matrix<T>&);
	
	int xDim, yDim, stride;

protected:
        bool owner;
        T* data;

private:
    static inline void _blockMultiply(int, int,int, const Matrix<T>&, const Matrix<T>&, Matrix<T>&);
	static void rowSwap(Matrix<T>&, const int, const int);
};
