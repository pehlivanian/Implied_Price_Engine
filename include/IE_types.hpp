//
// Created by charles on 10/15/16.
//

#ifndef __IE_TYPES_HPP__
#define __IE_TYPES_HPP__

#include <utility>
#include <forward_list>

#include "pool_allocator.hpp"


namespace IE_types
{

    using IPair = std::pair<int, std::pair<int, size_t>>;

    template<typename T1, typename T2>
    struct IPair_mut
    {
        IPair_mut(T1 f, T2 s) : f_(f), s_(s) {}

        T1& first()  { return f_; }
        T2& second() { return s_; }
        T1 f_;
        T2 s_;
    };

    template<class T, std::size_t N> using A = stack_allocator<T,N>;
    template<class T, std::size_t N> using ForwardList = std::forward_list<T, A<T,N>>;

}

#endif
