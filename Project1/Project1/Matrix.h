#pragma once

#include "Vector.cpp"

template<typename T> class Matrix
{
public:
	Matrix();
	Matrix(const Matrix<T>*);
	Matrix(const Matrix<T>&);
	Matrix(const int, const int);
	~Matrix();

	void initialize(int, int);
	
	static void Identity	(const int size  , Matrix<T>*				   );
	static void Identity	(const int x	 , const int y	   , Matrix<T>*);
	static void inverse		(const Matrix<T>*, Matrix<T>*				   );
	static void inverse		(Matrix<T>*		 , Matrix<T>*	   , T&		   );
	static void exponentiate(const Matrix<T>*, const int	   , Matrix<T>*);
	static void multiply	(const Matrix<T>*, const Matrix<T>*, Matrix<T>*);
	static void add			(const Matrix<T>*, const Matrix<T>*, Matrix<T>*);

	static void upperTriangulate(Matrix<T>*, Matrix<T>*, T&);

	Matrix<T> operator*(const Matrix<T>&)const;
	Matrix<T> operator+(const Matrix<T>&)const;
	Matrix<T> operator^(const int&)const;

	Matrix<T> operator*(const T&)const;
	Matrix<T> operator/(const T&)const;
	Matrix<T> operator+(const T&)const;
	Matrix<T> operator-(const T&)const;
	
	Vector<T> getColumn(const int)const;
	Vector<T> getRow(const int)const;

	T determinant() const;

	void randomize()const;
	void print()const;
	
	T* data;
	int xDim, yDim;

private:

	static void rowSwap(Matrix<T>*, const int, const int);
};
