#include <string.h>
#include <stdlib.h> 
#include <assert.h>

#include "Matrix.h"

#pragma region Constructors
template<typename T> Matrix<T>::Matrix()
{
    xDim = -1;
    yDim = -1;
    stride = -1;
    data = nullptr;
}

template<typename T> Matrix<T>::Matrix(const Matrix<T>& m)
{
    xDim = m.xDim;
    stride = m.xDim;
    yDim = m.yDim;
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
            data = new T[x*y];
        }
    }
    else
    {
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

template<typename T> void Matrix<T>::inverse(const Matrix<T>& m1, 
                                             Matrix<T>& inv)
{
    if (m1.xDim == m1.yDim){
        inv.initialize(m1.xDim, m1.yDim);

        if (m1.xDim == 2){
            T det = m1.determinant();
            if (det){
                inv(0, 0) = m1(1, 1) / det;
                inv(1, 0) = -m1(1, 0) / det;
                inv(0, 1) = -m1(0, 1) / det;
                inv(1, 1) = m1(1, 1) / det;
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

template<typename T> void Matrix<T>::inverse(Matrix<T>& m1,
                                             Matrix<T>& inv,
                                             T& det)
{
    printf("in\n");
    m1.print();

    Matrix<T>::upperTriangulate(m1, inv, det);

    printf("mid in\n");
    m1.print();

    printf("mid inv\n");
    inv.print();

    auto min = (m1.xDim < m1.yDim) ? m1.xDim : m1.yDim;

    for (int i = min - 1; i >= 0; i--){
        // From the bottom right corner of the upper triangle
        //  row reduce the elements of that column above it.

        Vector<T> invRow = inv.getRowView(i);
        Vector<T> m1Row = m1.getRowView(i);

        // scale this row by its element to get a value of 1
        invRow /= m1(i, i);
        m1(i, i) = (T)1;

        for (int j = 0; j < min; j++){
            // March throught the rows above it and eliminate 
            Vector<T> m1jRow = m1.getRowView(j);
            T& scale = m1jRow(i);

            m1jRow.scaleSubtract(m1Row, scale);
            inv.getRowView(j).scaleSubtract(invRow, scale);
        }
    }


    printf("in , I?\n");
    m1.print();

    printf("inv\n");
    inv.print();
}

template<typename T> void Matrix<T>::multiply(const Matrix<T>& m1,
                                              const Matrix<T>& m2,
                                              Matrix<T>& product)
{
    if (m1.xDim == m2.yDim){
        product.initialize(m2.xDim, m1.yDim);

        for (int y = 0; y < m1.yDim; y++){
            for (int x = 0; x < m2.xDim; x++){
                product(x, y) = 0;
                for (int inner = 0; inner < m1.xDim; inner++){
                    product(x, y) += m1(inner, y) * m2(x, inner);
                }
            }
        }
    }

    else
        throw  "MATRIX EXCEPTION: matrix multiplication dimention mismatch";
}

template<typename T> void Matrix<T>::add(const Matrix<T>& m1,
                                         const Matrix<T>& m2,
                                         Matrix<T>& sum)
{
    if (sum.data == nullptr)
        sum.initialize(m1.xDim, m1.y);



    if

    for (int i = 0; i < m1.xDim * m1.yDim; i++)
        sum(i) = m1(i) + m2(i);

}

template<typename T> void Matrix<T>::upperTriangulate(Matrix<T>& in,
                                                      Matrix<T>& out,
                                                      T& det)
{
    int xDim = in.xDim;
    //T* data = in.data;
    T co;

    Matrix<T>::Identity(xDim, in.yDim, out);
    det = (T)1;

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
            co = in(i, j) / in(i, i);

            // subtract the current row from the lower ones
            for (int x = 0; x < xDim; x++){
                in(x, j) -= co * in(x, i);
                out(x, j) -= co * in(x, i);
            }
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
    Matrix<T>& mtx = *this;

    if (xDim == yDim && xDim > 1){
        if (xDim == 2){
            det = mtx(0, 0) * mtx(1, 1) - mtx(1, 0) * mtx(0, 1);
        }
        else{

            Matrix<T> minor = Matrix<T>(xDim - 1, yDim - 1);
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

    Matrix<T>& mtx = *this;

    for (int i = 0; i < xDim * yDim; i++){
        mtx(i) = (T)((rand()) % 10) + 1;
    }
}

template<typename T> void Matrix<T>::print()
{
    Matrix<T>& mtx = *this;
    printf("xDim %d\nyDim %d\n", xDim, yDim);
    for (int y = 0; y < yDim; y++){
        printf("| ");
        for (int x = 0; x < xDim; x++){
            printf("%f ", mtx(x, y));
        }
        printf("|\n");
    }
    printf("\n");
}

