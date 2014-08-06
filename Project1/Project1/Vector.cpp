#include "Vector.h"


template<typename T> class Vector //: public Matrix<T>
{
public:
    Vector(T*, const int, const int);
    ~Vector();


    T& operator()(const int&)const;

};

template<typename T> Vector<T>::Vector(T* data, 
                                       const int stride, 
                                       const int width)
{
    this->data = data;
    this->stride = stride;

    if (stride == 1){
        xDim = width;
        yDim = 1;
    }
    else{
        xDim = 1;
        yDim = width;
    }
}


template<typename T>
Vector<T>::~Vector()
{
	delete data;
}


template<typename T> T&  Vector<T>::operator()(const int& idx) const{
    return *(data + idx * stride);
}

//
//template<typename T>
//inline void Vector<T>::add(const Vector<T>* v2){
//	int min = (width < v2->width) ? width : v2->width;
//
//	for (int i = 0; i < min; i++){
//		*(data + i*xStride) += ((T)v2->data + i*xStride)
//	}
//}
//template<typename T>
//inline void Vector<T>::subtract(const Vector<T>* v2){
//	int min = (width < v2->width) ? width : v2->width;
//
//	for (int i = 0; i < min; i++){
//		*(data + i*xStride) -= ((T)v2->data + i*xStride)
//	}
//}
//
//template<typename T>
//inline void Vector<T>::scaleSubtract(const Vector<T>& v2, const T& scale){
//	scaleSubtract(&v2, scale);
//}
//template<typename T>
//inline void Vector<T>::scaleSubtract(const Vector<T>* v2, const T& scale){
//	int min = (width < v2->width) ? width : v2->width;
//
//	for (int i = 0; i < min; i++){
//		*(data + i*xStride) -= ((T)v2->data + i*xStride) * scale;
//	}
//}
//
//template<typename T>
//inline void Vector<T>::divide(const T& d){
//
//	for (int i = 0; i < width; i++){
//		*(data + i*xStride) /= d;
//	}
//}
//
