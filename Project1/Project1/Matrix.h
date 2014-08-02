#pragma once




template<typename T> 
class Matrix
{
public:
	Matrix(int, int* );
	Matrix(int, int );
	~Matrix();

	Matrix* operator*(const Matrix*) const;
	Matrix* operator*(const T&) const;

	Matrix* operator+(const Matrix*) const;
	Matrix* operator+(const T&) const;

	Matrix* inverse() const;
	T determinant() const;

	T* data;
	int xDim, yDim;
};

