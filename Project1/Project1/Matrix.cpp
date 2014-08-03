#include "Matrix.h"

template<typename T> Matrix<T>::Matrix(int x, int y)
{
	xDim = x;
	yDim = y;

	data = new T[x*y];
}

template<typename T> Matrix<T>::~Matrix()
{
	delete data;
}


template<typename T> Matrix<T>* Matrix<T>::multiply(const Matrix<T>* m1, const Matrix<T>* m2)
{

	if (m1->xDim == m2->yDim){
		Matrix<T>* product = new Matrix<T>(m2->xDim, m1->yDim);

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

		return product;
	}
	throw "MATRIX EXCEPTION: matrix multiplication dimention mismatch";
}

template<typename T> Matrix<T>* Matrix<T>::add(const Matrix<T>* m1, const Matrix<T>* m2)
{

	if (m1->xDim == m2->yDim && m1->yDim == m2->yDim){
		Matrix<T>* result = new Matrix<T>(m1->xDim, m1->yDim);

		for (int i = 0; i < m1->xDim * m1->yDim; i++)
			result.data[i] = m1->data[i] + m2->data[i];

		return result;
	}
	throw "MATRIX EXCEPTION: matrix addition dimention mismatch";
}

template<typename T> void Matrix<T>::randomize()
{
	for (int i = 0; i < xDim * yDim; i++){
		data[i] = i;
	}

}

template<typename T> void Matrix<T>::print()
{
	for (int y = 0; y < yDim; y++){
		printf("| ");
		for (int x = 0; x < xDim; x++){
			printf("%f ", data[y*xDim + x]);
		}
		printf("|\n");
	}
}

template<typename T> T Matrix<T>::getDeterminant() const 
{
	if (xDim == yDim){
		if (xDim == 2)
			return data[0] * data[3] - data[1] * data[2];
		else
			throw "MATRIX EXCEPTION: large matrix Determinant not supported";
	}
	return 0;
}

template<typename T> Matrix<T>* Matrix<T>::inverse() const
{
	if (xDim == yDim){
		if (xDim == 2){

			T det = getDeterminant();
			if (det){
				Matrix<T>* inv = new Matrix<T>(xDim, yDim);
				inv->data[0] = data[3] / det; 
				inv->data[1] = -data[1] / det;
				inv->data[2] = -data[2] / det;
				inv->data[3] = data[0] / det;

				return inv;
			}
		}
		else
		{
			throw "MATRIX EXCEPTION: large matrix Inverse not supported";
		}
	}
	throw "MATRIX EXCEPTION: matrix Inverse does not exist";
}

//template<typename T> Matrix<T> Matrix<T>::operator*(const T& scaler) const
//{
//	Matrix<T> result = Matrix<T>(xDim, yDim);
//
//	for (int i = 0; i < xDim * yDim; i++)
//		result.data[i] = data[i] * scaler;
//
//	return result;
//
//}
//
//
//template<typename T> Matrix<T> Matrix<T>::operator+(const T& scaler) const
//{
//	Matrix<T> result = Matrix<T>(xDim, yDim);
//
//	for (int i = 0; i < xDim * yDim; i++)
//		result.data[i] = data[i] + scaler;
//
//	return result;
//
//}
//
//
//
//template<typename T> Matrix<T> operator* (const T& scaler, const Matrix<T>& m) 
//{
//	return m * scaler;
//}