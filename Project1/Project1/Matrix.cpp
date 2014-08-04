#include "Matrix.h"
#include <string.h>
#include <stdlib.h>
#include <stdexcept>    

template<typename T> Matrix<T>::Matrix()
{
	xDim = -1;
	yDim = -1;
	data = nullptr;
}

template<typename T> Matrix<T>::Matrix(const Matrix<T>& m)
{
	xDim = m.xDim;
	yDim = m.yDim;
	data = new T[xDim*yDim];
	memcpy(data, m.data, sizeof(T)*xDim*yDim);
}

template<typename T> Matrix<T>::Matrix(const Matrix<T>* m)
{
	xDim = m->xDim;
	yDim = m->yDim;
	data = new T[xDim*yDim];
	memcpy(data, m->data, sizeof(T)*xDim*yDim);
}

template<typename T> Matrix<T>::Matrix(const int x, 
									   const int y)
{
	initialize(x, y);
}

template<typename T> Matrix<T>::~Matrix()
{
	delete data;
}

template<typename T> void Matrix<T>::initialize(const int x, 
												const int y)
{
	
	if (data){
		// Matrix has been initialized before.
		if (x * y == xDim * yDim) {
			// the Data array is the right size
			memset(data, 0, x*y * sizeof(T));
		}
		else
		{
			// The data array is wht wrong size
			delete data;
			data = new T[x*y];
		}
	}
	else
	{
		data = new T[x*y];
	}

	xDim = x;
	yDim = y;	
}

template<typename T> void Matrix<T>::Identity(const int size,
											  Matrix<T>* id)
{
	id->initialize(size, size);
	for (int i = 0; i < size; i++)
		id->data[i* size + i] = 1;
}

template<typename T> void Matrix<T>::exponentiate(const Matrix<T>* m1,
												  const int power,
												  Matrix<T>* product)
{

	if (power == 0){
		Matrix<T>::Identity(m1->xDim, product);
		
	}
	else{
		int p = power, i = 1;
		Matrix<T> temp = Matrix<T>(m1);
		Matrix<T> * p1 = product, *p2 = &temp, *p3;
		if (p < 0){
			Matrix<T>::inverse(m1, p1);
			p3 = p1;
			p1 = p2;
			p2 = p3;
			p = -p;
		}
		while (i++ < p){
			Matrix<T>::multiply(p2, m1, p1);
			p3 = p1;
			p1 = p2;
			p2 = p3;
		}
		if (p1 == product){
			memcpy(product->data, temp.data, product->xDim * product->yDim * sizeof(T));
		}
	}
}

template<typename T> void Matrix<T>::inverse(const Matrix<T>* m1, Matrix<T>* inv)
{
	if (m1->xDim == m1->yDim){
		inv->initialize(m1->xDim, m1->yDim);

		if (m1->xDim == 2){
			T det = m1->determinant();
			if (det){
				inv->data[0] = m1->data[3] / det;
				inv->data[1] = -m1->data[1] / det;
				inv->data[2] = -m1->data[2] / det;
				inv->data[3] = m1->data[0] / det;
				return;
			}
		}
		else
		{
			throw "MATRIX EXCEPTION: large matrix Inverse not supported";
		}
	}
	else
	{
		throw "MATRIX EXCEPTION: matrix Inverse does not exist";
	}
}

template<typename T> void Matrix<T>::multiply(const Matrix<T>* m1, 
											  const Matrix<T>* m2, 
											  Matrix<T>* product)
{
	if (m1->xDim == m2->yDim){
		product->initialize(m2->xDim, m1->yDim);

		int yOffset;
		for (int y = 0; y < m1->yDim; y++){
			yOffset = y*m2->xDim;
			for (int x = 0; x < m2->xDim; x++){
				product->data[yOffset + x] = 0;
				for (int inner = 0; inner < m1->xDim; inner++){
					product->data[yOffset + x] += m1->data[yOffset + inner] * m2->data[inner * m2->xDim + x];
				}
			}
		}
	}
	
	else
		throw  "MATRIX EXCEPTION: matrix multiplication dimention mismatch";
}

template<typename T> void Matrix<T>::add(const Matrix<T>* m1, 
										 const Matrix<T>* m2, 
										 Matrix<T>* sum)
{
	if (m1->xDim == m2->yDim && m1->yDim == m2->yDim){
		sum->initialize(m1->xDim,m1->yDim)

		for (int i = 0; i < m1->xDim * m1->yDim; i++)
			sum.data[i] = m1->data[i] + m2->data[i];
	}
	throw "MATRIX EXCEPTION: matrix addition dimention mismatch";
}

template<typename T> T Matrix<T>::determinant() const 
{
	T det = 0;
	if (xDim == yDim && xDim > 1){
		if (xDim == 2){
			det = data[0] * data[3] - data[1] * data[2];
		}
		else{

			Matrix<T> minor = Matrix<T>(xDim - 1, yDim - 1);
			int sign = 1;
			for (int idx = 0; idx < xDim; idx++){
				if (data[idx] != 0){
					for (int yMinor = 0, yMajor = 1; yMinor < minor.yDim; yMinor++, yMajor++){
						for (int xMinor = 0,xMajor = 0; xMinor < minor.xDim; xMinor++,xMajor++){
							if (xMajor == idx)
								xMajor++;
							minor.data[yMinor *minor.xDim + xMinor] = data[(yMajor)*xDim + xMajor];
						}
					}
					det += sign * data[idx] * minor.determinant();
				}
				sign *= -1;
			}
		}
	}
	return det;
}

template<typename T> Matrix<T> Matrix<T>::operator*(const Matrix<T>& m2)const
{
	Matrix<T> product = Matrix<T>();
	Matrix<T>::multiply(this, &m2, &product);
	return product;
}

template<typename T> Matrix<T> Matrix<T>::operator^(const int& power)const
{
	Matrix<T> product = Matrix<T>();
	Matrix<T>::exponentiate(this, power, &product);
	return product;
}

template<typename T> Matrix<T> Matrix<T>::operator*(const T& scaler) const
{
	Matrix<T> result = Matrix<T>(xDim, yDim);

	for (int i = 0; i < xDim * yDim; i++)
		result.data[i] = data[i] * scaler;

	return result;
}

template<typename T> Matrix<T> Matrix<T>::operator+(const T& scaler) const
{
	Matrix<T> result = Matrix<T>(xDim, yDim);

	for (int i = 0; i < xDim * yDim; i++)
		result.data[i] = data[i] + scaler;

	return result;
}

template<typename T> Matrix<T> Matrix<T>::operator/(const T& scaler) const
{
	Matrix<T> result = Matrix<T>(xDim, yDim);

	for (int i = 0; i < xDim * yDim; i++)
		result.data[i] = data[i] / scaler;

	return result;
}

template<typename T> Matrix<T> Matrix<T>::operator-(const T& scaler) const
{
	Matrix<T> result = Matrix<T>(xDim, yDim);

	for (int i = 0; i < xDim * yDim; i++)
		result.data[i] = data[i] - scaler;

	return result;
}



template<typename T> void Matrix<T>::randomize()const
{
	for (int i = 0; i < xDim * yDim; i++){
		data[i] = (T)((rand())%10);
	}
}

template<typename T> void Matrix<T>::print() const
{
	for (int y = 0; y < yDim; y++){
		printf("| ");
		for (int x = 0; x < xDim; x++){
			printf("%f ", data[y*xDim + x]);
		}
		printf("|\n");
	}
}
