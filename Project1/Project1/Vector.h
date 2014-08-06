#pragma once
#include <string.h>
#include <stdlib.h>
#include "Matrix.cpp"

template<typename T>
class Vector : public Matrix<T>
{
public:
	Vector(const int);
	Vector( T*,const int, const int);
	~Vector();

	T get(const int);
	void set(const int, const T&);
	void add(const Vector<T>*);
	void subtract(const Vector<T>*);
	void scaleSubtract(const Vector<T>&, const T&);
	void scaleSubtract(const Vector<T>*, const T&);
	void divide(const T&);

	T* data;
	int width;
private:
	int xStride;
};

