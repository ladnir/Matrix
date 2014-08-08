#pragma once

template<typename T> class Matrix;

template<typename T> class Vector : public Matrix<T>
{
public:
    Vector(const int);
    Vector(T*, const int, const int);
    ~Vector();


    T& operator()(const int&)const;

};
