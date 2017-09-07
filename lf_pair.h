//
// Created by van on 17-9-7.
//

#ifndef TINY_STL_LF_PAIR_H
#define TINY_STL_LF_PAIR_H

namespace lf {
    template <typename T1, typename T2>
    struct pair {
        typedef T1 first_type;
        typedef T2 second_type;

        T1 first;
        T2 second;

        pair(): first(T1()), second(T2()) {}
        pair(const T1& _first, const T2& _second): first(_first), second(_second) {}
    };
}

#endif //TINY_STL_LF_PAIR_H
