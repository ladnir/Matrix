#pragma once

template<typename T> class Matrix
{
private:
	Matrix();
public:
	Matrix(int, int);
	~Matrix();

	static Matrix<T>* multiply(const Matrix<T>*, const Matrix<T>*);
	static Matrix<T>* add(const Matrix<T>*, const Matrix<T>*);


	Matrix<T>* inverse() const;
	T getDeterminant() const;
	void randomize();
	void print();


	T* data;
	int xDim, yDim;
};
