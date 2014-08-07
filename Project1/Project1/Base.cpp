#include "Base.h"

template<class T>
Base<T>::Base()
{

    xDim = 1;
}

template<class T>
Base<T>::~Base()
{
    
}

template<class T> Child<T> Base<T>::MakeChild(){
    return Child<T>();
}
