//#include "Vector.h"
//
//template<typename T> Vector<T>::Vector(const int size){
//
//    data = (T*)malloc(sizeof(T)* size);
//    xDim = size;
//    yDim = 1;
//    stride = 0;
//
//}
//
//
//template<typename T> Vector<T>::Vector(T* data,
//    const int stride,
//    const int width)
//{
//    this->data = data;
//    this->stride = stride;
//
//    if (stride == 1){
//        xDim = width;
//        yDim = 1;
//    }
//    else{
//        xDim = 1;
//        yDim = width;
//    }
//}
//
//
//template<typename T>
//Vector<T>::~Vector()
//{
//    delete data;
//}
//
//
//template<typename T> T&  Vector<T>::operator()(const int& idx) const{
//    return *(data + idx * stride);
//}
