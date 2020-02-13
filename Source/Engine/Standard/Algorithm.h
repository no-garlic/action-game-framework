
#ifndef __STD_ALGORITHM_H_
#define __STD_ALGORITHM_H_

#include "Type.h"

template <typename T>
inline Array<T> &operator+=(Array<T> &copyTo, const Array<T> &copyFrom)
{
    for (uint i = 0; i < copyFrom.size(); i++)
    {
        copyTo.push_back(copyFrom[i]);
    }

    return copyTo;
}

template <typename T>
inline void ClearQueue(Queue<T> &q)
{
    while (!q.empty())
    {
        q.pop();
    }
}

#endif // __STD_ALGORITHM_H_
