#pragma once

#include "Child.h"

template<class T> class Base
{
public:
    Base();
    ~Base();

    int xDim, yDim;

    Child<T> MakeChild();
};

