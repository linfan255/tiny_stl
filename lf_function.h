//
// Created by van on 17-9-7.
//

#ifndef TINY_STL_LF_FUNCTION_H
#define TINY_STL_LF_FUNCTION_H

namespace lf {
    template <typename T>
    struct less {
        bool operator()(const T& lhs, const T& rhs) const {
            return lhs < rhs;
        }
    };

    template <typename T>
    struct identity {
        const T& operator()(const T& x) const {
            return x;
        }
    };
}

#endif //TINY_STL_LF_FUNCTION_H
