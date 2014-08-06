#include "Vector.h"

template<typename T>
Vector<T>::Vector(const int size)
{
	data = (T*)malloc(sizeof(T)*size);
	width = size;
}

template<typename T>
Vector<T>::Vector( T* d, const int x, const int w)
{
	data = d;
	xStride = x;
	width = w;
}

template<typename T>
Vector<T>::~Vector()
{
	delete data;
}


template<typename T> 
inline T Vector<T>::get(const int idx){
	
	return *(data + idx*xStride);
}

template<typename T>
inline void Vector<T>::set(const int idx, const T& item){
	*(data + idx*xStride) = item;
}

template<typename T>
inline void Vector<T>::add(const Vector<T>* v2){
	int min = (width < v2->width) ? width : v2->width;

	for (int i = 0; i < min; i++){
		*(data + i*xStride) += ((T)v2->data + i*xStride)
	}
}
template<typename T>
inline void Vector<T>::subtract(const Vector<T>* v2){
	int min = (width < v2->width) ? width : v2->width;

	for (int i = 0; i < min; i++){
		*(data + i*xStride) -= ((T)v2->data + i*xStride)
	}
}

template<typename T>
inline void Vector<T>::scaleSubtract(const Vector<T>& v2, const T& scale){
	scaleSubtract(&v2, scale);
}
template<typename T>
inline void Vector<T>::scaleSubtract(const Vector<T>* v2, const T& scale){
	int min = (width < v2->width) ? width : v2->width;

	for (int i = 0; i < min; i++){
		*(data + i*xStride) -= ((T)v2->data + i*xStride) * scale;
	}
}

template<typename T>
inline void Vector<T>::divide(const T& d){

	for (int i = 0; i < width; i++){
		*(data + i*xStride) /= d;
	}
}

