//
// Created by charles on 10/12/16.
//

#ifndef __IMPL_HPP__
#define __IMPL_HPP__

template<typename T>
struct impl
{
    T* backpointer;
};

template<typename S, template<typename> class T>
struct impl_ST
{
    T<S>* backpointer;
};

template<typename S, typename U, template<typename, typename> class T>
struct impl_STU
{
    T<S,U>* backpointer;
};

#endif