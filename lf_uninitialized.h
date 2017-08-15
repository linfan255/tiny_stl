//
// Created by van on 17-8-11.
//

#ifndef TINY_STL_LF_UNINITIALIZED_H
#define TINY_STL_LF_UNINITIALIZED_H

#include <cstring>
#include "lf_type_traits.h"
#include "lf_iterator.h"
#include "lf_construct.h"

namespace lf{
    /************uninitialized_fill_n*****************/
    //针对is_pod_type的_true_type _false_type的两个特话版本
    template <typename ForwardIterator, typename Size, typename T>
    inline ForwardIterator _uninitialized_fill_n_aux(ForwardIterator start, Size n, const T& val,
                                                _true_type);

    template <typename ForwardIterator, typename Size, typename T>
    inline ForwardIterator _uninitialized_fill_n_aux(ForwardIterator start, Size n, const T& val,
                                                 _false_type);

    template <typename ForwardIterator, typename Size, typename T>
    inline ForwardIterator uninitialized_fill_n(ForwardIterator start, Size n, const T& val){
        typedef typename type_traits<T>::is_pod_type is_pod_type;
        return _uninitialized_fill_n_aux(start, n, val, is_pod_type());
    }

    template <typename ForwardIterator, typename Size, typename T>
    inline ForwardIterator _uninitialized_fill_n_aux(ForwardIterator start, Size n, const T& val,
                                                 _false_type) {
        ForwardIterator it;
        for(it = start; n > 0; n--, it++)
            construct((T*)&it, val);
        return it;
    }

    template <typename ForwardIterator, typename Size, typename T>
    inline ForwardIterator _uninitialized_fill_n_aux(ForwardIterator start, Size n, const T& val,
                                                     _true_type){
        ForwardIterator it;
        for(it = start; n > 0; n--, it++)
            *it = val;
        return it;
    }


    /************************uninitialized_copy*********************/
    template <typename InputIterator, typename ForwardIterator>
    inline ForwardIterator _uninitialized_copy_aux(InputIterator start, InputIterator finish,
                                                   ForwardIterator dst, _true_type);

    template <typename InputIterator, typename ForwardIterator>
    inline ForwardIterator _uninitialized_copy_aux(InputIterator start, InputIterator finish,
                                                   ForwardIterator dst, _false_type);

    template <typename InputIterator, typename ForwardIterator>
    inline ForwardIterator uninitialized_copy(InputIterator start, InputIterator finish,
                                              ForwardIterator dst) {
        typedef typename iterator_traits<InputIterator>::value_type value_type;
        typedef typename type_traits<value_type>::is_pod_type is_pod;
        return _uninitialized_copy_aux(start, finish, dst, is_pod());
    }

    template <typename InputIterator, typename ForwardIterator>
    inline ForwardIterator _uninitialized_copy_aux(InputIterator start, InputIterator finish,
                                                   ForwardIterator dst, _true_type) {
        typedef typename iterator_traits<InputIterator>::value_type value_type;
        memcpy(&*dst, &*start, sizeof(value_type) * (finish - start));
        return dst + (finish - start);
    }

    template <typename InputIterator, typename ForwardIterator>
    inline ForwardIterator _uninitialized_copy_aux(InputIterator start, InputIterator finish,
                                                   ForwardIterator dst, _false_type){
        ForwardIterator ret = dst;
        for(; start != finish; start++, ret++)
            construct(&*ret, *start);
        return ret;
    }

    /**************uninitialized_fill****************/
    template <typename ForwardIteraotr, typename T>
    inline void _uninitialized_fill_aux(ForwardIteraotr start, ForwardIteraotr finish,
                                                    const T& x, _true_type);

    template <typename ForwardIteraotr, typename T>
    inline void _uninitialized_fill_aux(ForwardIteraotr start, ForwardIteraotr finish,
                                                   const T& x, _false_type);

    template <typename ForwardIterator, typename T>
    inline void uninitialized_fill(ForwardIterator start, ForwardIterator finish, const T& x){
        typedef typename type_traits<T>::is_pod_type is_pod;
        _uninitialized_fill_aux(start, finish, x, is_pod());
    }

    template <typename ForwardIteraotr, typename T>
    inline void _uninitialized_fill_aux(ForwardIteraotr start, ForwardIteraotr finish,
                                                   const T& x, _true_type){
        ForwardIteraotr it;
        for(it = start; it != finish; it++) {
            *it = x;
        }
    }

    template <typename ForwardIteraotr, typename T>
    inline void _uninitialized_fill_aux(ForwardIteraotr start, ForwardIteraotr finish,
                                        const T& x, _false_type){
        ForwardIteraotr it;
        for(it = start; it != finish; it++)
            construct(&*it, x);
    }
}

#endif //TINY_STL_LF_UNINITIALIZED_H
