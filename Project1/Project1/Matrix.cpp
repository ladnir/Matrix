#include <string.h>
#include <stdlib.h> 
#include <assert.h>
#include <cmath>
#include "Matrix.h"

#pragma region Constructors
template<typename T> Matrix<T>::Matrix()
{
    xDim = -1;
    yDim = -1;
    stride = -1;
    owner =  false;
    data = nullptr;
}

template<typename T> Matrix<T>::Matrix(const Matrix<T>& m)
{
    xDim = m.xDim;
    stride = m.xDim;
    yDim = m.yDim;
    owner = true;
    data = new T[xDim*yDim];
    memcpy(data, m.data, sizeof(T)*xDim*yDim);
}

template<typename T> Matrix<T>::Matrix(const int x,
                                       const int y)
{
    data = nullptr;
    initialize(x, y);
}

template<typename T> Matrix<T>::~Matrix()
{
    if (owner)
        delete data;
}
#pragma endregion 

#pragma region Static Functions
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
            owner = true;
            data = new T[x*y];
        }
    }
    else
    {
        owner = true;
        data = new T[x*y];
    }

    xDim = x;
    stride = x;
    yDim = y;
}

template<typename T> void Matrix<T>::Identity(const int size,
                                              Matrix<T>& id)
{
    Matrix<T>::Identity(size, size, id);
}

template<typename T> void Matrix<T>::Identity(const int x,
                                              const int y,
                                              Matrix<T>& id)
{
    auto min = (x < y) ? x : y;

    id.initialize(x, y);
    for (int i = 0; i < y; i++)
    for (int j = 0; j<x; j++)
        id(j, i) = 0;

    for (int i = 0; i < min; i++)
        id(i, i) = 1;
}

template<typename T> void Matrix<T>::exponentiate(const Matrix<T>& m1,
                                                  const int power,
                                                  Matrix<T>& product)
{
    if (power < -1)
        throw "Not implemented exception";

    if (power == 0){
        Matrix<T>::Identity(m1.xDim, product);

    }
    else{
        int p = power, i = 1;
        Matrix<T> temp = Matrix<T>(m1);
        Matrix<T>& p1 = product, &p2 = temp, &p3;
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
            memcpy(product.data, temp.data, product.xDim * product.yDim * sizeof(T));
        }
    }
}

template<typename T> void Matrix<T>::inverse(Matrix<T>& m1,
                                             Matrix<T>& inv,
                                             T& det)
{
    assert(m1.xDim == m1.yDim);
    
    upperTriangulate(m1, inv, det);
    auto min = (m1.xDim < m1.yDim) ? m1.xDim : m1.yDim;

    for (int i = min - 1; i >= 0; i--){
        // From the bottom right corner of the upper triangle
        //  row reduce the elements of that column above it.

        auto invRow = inv.getRowView(i);
        auto m1Row = m1.getRowView(i);

        // scale this row by its element to get a value of 1
        invRow /= m1(i, i);
        m1(i, i) = 1;

        for (int j = 0; j < i; j++){
            // March throught the rows above it and eliminate 
            auto m1jRow = m1.getRowView(j);
            auto scale = m1jRow(i);

            m1jRow.scaleSubtract(m1Row, scale);
            inv.getRowView(j).scaleSubtract(invRow, scale);
        }
    }
}

template<typename T> void Matrix<T>::transpose(const Matrix<T>& in,
                                               Matrix<T>& trans)
{
    if (trans.data == nullptr)
        trans.initialize(in.yDim, in.xDim);

    int yOff;
    for (int y = 0; y < in.yDim; y++){
        yOff = y * in.xDim;
        for (int x = 0; x < in.xDim; x++){
            trans(y, x) = in(yOff + x);
        }
    }
}

template<typename T> void Matrix<T>::multiply(const Matrix<T>& m1,
    const Matrix<T>& m2,
    Matrix<T>& product)
{
    assert(m1.xDim == m2.yDim);

    Matrix<T> m2Trans;
    transpose(m2, m2Trans);

    product.initialize(m2.xDim, m1.yDim);

    //float sum = 0;
    int yOff, yTranOff;
    for (int y = 0; y < m1.yDim; y++){
        yOff = y * product.xDim;
        for (int x = 0; x < m2.xDim; x++){
            yTranOff = x * m2.yDim;
            product(yOff + x) = 0;

            for (int inner = 0; inner < m1.xDim; inner++){
                product(yOff + x) += m1(yOff + inner) * m2Trans(yTranOff + inner);
            }
            //sum += product(yOff + x);
        }
    }
    //assert(32483663872.0 == sum);

}

template<typename T> void Matrix<T>::blockMultiply(const Matrix<T>& m1,
    const Matrix<T>& m2,
    Matrix<T>& product)
{
    assert(m1.xDim == m2.yDim);

    Matrix<T> m2Trans;
    transpose(m2, m2Trans);

    product.initialize(m2.xDim, m1.yDim);

    _blockMultiply(0,0,m1.xDim,m1,m2Trans,product);

    float sum = 0 ;
    for (int i = 0; i < m1.xDim*m1.yDim; i++){
        sum += product(i);
    }
    assert(32483663872.0 == sum);

}


#define TopLeft     0
#define TopRight    1
#define BottmLeft   2
#define BottomRight 3
#define Done		4

template<typename T> void Matrix<T>::blockMultiply2(const Matrix<T>& m1,
												   const Matrix<T>& m2,
	 											   Matrix<T>& product)
{
	if (product.data == nullptr)
		product.initialize(m2.xDim, m1.yDim);

	int depth = (int)log2(m1.xDim) + 1; // fix assumption
	
	int states[11]; // = (int*)malloc(sizeof(int)* depth);
	int Xs    [11]; // = (int*)malloc(sizeof(int)* depth);
	int Ys    [11]; // = (int*)malloc(sizeof(int)* depth);
	int Widths[11]; // = (int*)malloc(sizeof(int)* depth);

	int myDepth = 0;
	states[0] = TopLeft;
	Xs[0]     = 0;
	Ys[0]     = 0;
	Widths[0] = m1.xDim; // fix assumption

	Matrix<T> m2Trans;
	transpose(m2, m2Trans);
	T sum = 0;

	while (myDepth >= 0){
		if (myDepth < 4)
			printf("%d,%d,%d   --  %d\n", states[0],states[1],states[2], myDepth);
		//printf("d%d\tw%d\ts%d\t(%d,%d)\n", myDepth, Widths[myDepth],states[myDepth], Xs[myDepth], Ys[myDepth]);
		switch (states[myDepth])
		{
		case TopLeft:
			if (Widths[myDepth] == 1){
				int x = Xs[myDepth], y = Ys[myDepth];
				// Hit base case, compute product and unwind recurrion to the previous depth;
				//printf("d%d\t(%d,%d)\n", myDepth,Xs[myDepth], Ys[myDepth]);
				product(x, y ) = 0;
				for (int inner = 0; inner < m1.xDim; inner++){
					product(x, y) += m1(inner, y) * m2Trans(inner, x);
				}
				sum += product(x, y);
				myDepth--;
				break;
			}

			// recurs deeper on the top left quarter of the matrix.
			Xs[myDepth + 1] = Xs[myDepth];
			Ys[myDepth + 1] = Ys[myDepth];
			states[myDepth]++;
			states[myDepth + 1] = 0;
			Widths[myDepth + 1] = Widths[myDepth] / 2;
			myDepth++;


			break;
		case TopRight:
			// recurs deeper on the top right quarter of the matrix.
			Ys[myDepth + 1] = Ys[myDepth];
			Xs[myDepth + 1] = Xs[myDepth] + Widths[myDepth] / 2;
			states[myDepth]++;
			states[myDepth + 1] = 0;
			Widths[myDepth + 1] = Widths[myDepth] / 2;
			myDepth++;


			break;
		case BottmLeft:
			// recurs deeper on the bottom left quarter of the matrix.
			Xs[myDepth + 1] = Xs[myDepth];
			Ys[myDepth + 1] = Ys[myDepth] + Widths[myDepth] / 2;
			states[myDepth]++;
			states[myDepth + 1] = 0;
			Widths[myDepth + 1] = Widths[myDepth] / 2;
			myDepth++;

			break;
		case BottomRight:
			// recurs deeper on the bottom right quarter of the matrix.
			Xs[myDepth + 1] = Xs[myDepth] + Widths[myDepth] / 2;
			Ys[myDepth + 1] = Ys[myDepth] + Widths[myDepth] / 2;
			states[myDepth]++;
			states[myDepth + 1] = 0;
			Widths[myDepth + 1] = Widths[myDepth] / 2;
			myDepth++;


			break;
		case Done:
			// unwind recurrion to the previous depth;
			myDepth--;
			break;
		default:
			assert(0 && "default reached");
			break;
		}
	}

	//assert(32483663872.0 == sum);

}

template<typename T> void Matrix<T>::_blockMultiply(int xStart, 
                                               int yStart, int width,
                                               const Matrix<T>& m1,
                                               const Matrix<T>& m2,
                                               Matrix<T>& product)
{
    if (width == 1){
        product(xStart, yStart) = 0;
        for (int inner = 0; inner < m1.xDim; inner++){
            product( xStart, yStart) += m1(inner, yStart) * m2(inner, xStart);
        }
    }
    else{
        int hWidth = width / 2;
        _blockMultiply(xStart         , yStart         , hWidth, m1, m2, product);
        _blockMultiply(xStart + hWidth, yStart         , hWidth, m1, m2, product);
        _blockMultiply(xStart         , yStart + hWidth, hWidth, m1, m2, product);
        _blockMultiply(xStart + hWidth, yStart + hWidth, hWidth, m1, m2, product);
    }
}
template<typename T> void Matrix<T>::add(const Matrix<T>& m1,
                                         const Matrix<T>& m2,
                                         Matrix<T>& sum)
{
    assert(m1.xDim == m2.xDim && m1.yDim == m2.yDim);
    
    if (sum.data == nullptr)
        sum.initialize(m1.xDim, m1.yDim);
        
    for (int i = 0; i < m1.xDim * m1.yDim; i++)
        sum(i) = m1(i) + m2(i);

}

template<typename T> void Matrix<T>::upperTriangulate(Matrix<T>& in,
                                                      Matrix<T>& out,
                                                      T& det)
{
    assert(in.xDim == out.xDim && in.yDim == out.yDim);

    auto xDim = in.xDim;
    T scale;

    Matrix<T>::Identity(xDim, in.yDim, out);
    det = (T)-1;

    for (int i = 0; i < xDim; i++){
        if (in(i, i) == 0){
            // This row has a zero in the main diagonal.
            // Lets try and find a row to swap it with or return 
            for (int j = i + 1; j < xDim; j++){
                if (in(i, j) == 0){
                    rowSwap(in, i, j);
                    rowSwap(out, i, j);
                    break;
                }
            }
            if (in(i, i) == 0){
                det = (T)0;
                return;
            }
        }

        det *= in(i, i);

        for (int j = i + 1; j < in.yDim; j++){
            scale = in(i, j) / in(i, i);
            // subtract the current row from the lower ones
            in.getRowView(j).scaleSubtract(in.getRowView(i), scale);
            out.getRowView(j).scaleSubtract(out.getRowView(i), scale);
        }

    }
}

template<typename T> void Matrix<T>::rowSwap(Matrix<T>& m1,
                                            const int row1,
                                            const int row2)
{
    assert(row1 < m1.xDim && row2 < m1.xDim);
    
    T temp;
    for (int i = 0; i < m1.xDim; i++){
        temp = m1(i, row1);
        m1(i, row1) = m1(i, row2);
        m1(i, row2) = temp;
    }
}

#pragma endregion

template<typename T> void Matrix<T>::scaleSubtract(const Matrix<T>& subtract, const T& scale){

    assert(this->xDim == subtract.xDim && this->yDim == subtract.yDim);

    for ( int i = 0; i< xDim * yDim; i++)
        this->operator()(i) -= subtract(i) * scale; 
}

template<typename T> T Matrix<T>::determinant() const
{
    T det = 0;
    auto& mtx = *this;

    if (xDim == yDim && xDim > 1){
        if (xDim == 2){
            det = mtx(0, 0) * mtx(1, 1) - mtx(1, 0) * mtx(0, 1);
        }
        else{

            auto minor = Matrix<T>(xDim - 1, yDim - 1);
            int sign = 1;
            for (int idx = 0; idx < xDim; idx++){
                if (mtx(idx, 0) != 0){
                    for (int yMinor = 0, yMajor = 1; yMinor < minor.yDim; yMinor++, yMajor++){
                        for (int xMinor = 0, xMajor = 0; xMinor < minor.xDim; xMinor++, xMajor++){
                            if (xMajor == idx)
                                xMajor++;
                            minor(xMinor, yMinor) = mtx(xMajor, yMajor);
                        }
                    }
                    det += sign * mtx(idx, 0) * minor.determinant();
                }
                sign *= -1;
            }
        }
    }
    return det;
}

#pragma region operators
template<typename T> T&  Matrix<T>::operator()(const int& x, const int& y) const{
    return *(data + y * stride + x);
}

template<typename T> T&  Matrix<T>::operator()(const int& idx) const{
    return *(data + idx);
}

template<typename T> Matrix<T> Matrix<T>::operator*(const Matrix<T>& m2)const
{
    Matrix<T> product = Matrix<T>();
    Matrix<T>::multiply(*this, m2, product);
    return product;
}

template<typename T> Matrix<T> Matrix<T>::operator^(const int& power)const
{
    Matrix<T> product = Matrix<T>();
    Matrix<T>::exponentiate(*this, power, product);
    return product;
}

template<typename T> Matrix<T> Matrix<T>::operator*(const T& scaler) const
{
    Matrix<T> result = Matrix<T>(xDim, yDim);
    Matrix<T>& mtx = *this;

    for (int i = 0; i < xDim * yDim; i++)
        result(i) = mtx(i) * scaler;

    return result;
}

template<typename T> Matrix<T> Matrix<T>::operator+(const T& scaler) const
{
    Matrix<T> result = Matrix<T>(xDim, yDim);
    Matrix<T>& mtx = *this;

    for (int i = 0; i < xDim * yDim; i++)
        result(i) = mtx(i) + scaler;

    return result;
}

template<typename T> Matrix<T> Matrix<T>::operator/(const T& scaler) const
{
    Matrix<T> result = Matrix<T>(xDim, yDim);
   // Matrix<T>& mtx = *this;

    for (int i = 0; i < xDim * yDim; i++)
        result(i) = this->operator()(i) / scaler;

    return result;
}

template<typename T> Matrix<T>& Matrix<T>::operator/=(const T& scaler)
{
    for (int i = 0; i < xDim * yDim; i++)
        this->operator()(i) /= scaler;

    return *this;
}

template<typename T> Matrix<T>& Matrix<T>::operator*=(const T& scaler)
{
    for (int i = 0; i < xDim * yDim; i++)
        this->operator()(i) *= scaler;

    return *this;
}

template<typename T> Matrix<T>& Matrix<T>::operator+=(const T& scaler)
{
    for (int i = 0; i < xDim * yDim; i++)
        this->operator()(i) += scaler;

    return *this;
}

template<typename T> Matrix<T>& Matrix<T>::operator-=(const T& scaler)
{
    for (int i = 0; i < xDim * yDim; i++)
        this->operator()(i) = scaler;

    return *this;
}

template<typename T> Matrix<T> Matrix<T>::operator-(const T& scaler) const
{
    Matrix<T> result = Matrix<T>(xDim, yDim);
    Matrix<T>& mtx = *this;

    for (int i = 0; i < xDim * yDim; i++)
        result(i) = mtx(i) - scaler;

    return result;
}
#pragma endregion Matrix class operators

template<typename T> Vector<T> Matrix<T>::getColumnView(const int col)const{

    return Vector<T>(data + col, xDim, yDim);
}

template<typename T> Vector<T> Matrix<T>::getRowView(const int row)const{
    return Vector<T>(data + row * stride, 1, xDim);
}


template<typename T> void Matrix<T>::randomize()
{
    for (int i = 0; i < xDim * yDim; i++){
        this->operator()(i) = (T)(rand() % 10) + 1;
    }
}

template<typename T> void Matrix<T>::print()
{
    printf("xDim %d\nyDim %d\n", xDim, yDim);
    for (int y = 0; y < yDim; y++){
        for (int x = 0; x < xDim; x++){
            printf("%4.2f\t", data[xDim * y + x]);
        }
        printf("\n");
    }
    printf("________________________\n");
}

template<typename T> void Matrix<T>::print(Matrix<T>& m2)
{
    printf("xDim %d\nyDim %d\n", xDim, yDim);
    for (int y = 0; y < yDim; y++){
        for (int x = 0; x < xDim; x++){
            printf("%4.2f\t", data[xDim * y + x]);
        }

        printf("|\t");
        for (int x = 0; x < xDim; x++){
            printf("%4.2f\t", m2(x,y));
        }
        printf("\n");
    }
    printf("________________________\n");
}

