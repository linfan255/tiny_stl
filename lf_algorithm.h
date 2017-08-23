//
// Created by van on 17-8-12.
//

#include <cstring>
#include "lf_type_traits.h"
#include "lf_iterator.h"

#ifndef TINY_STL_LF_ALGORITHM_H
#define TINY_STL_LF_ALGORITHM_H


namespace lf {
    /********************************实现copy*******************************************/
    template <typename InputIterator, typename OutputIterator, typename Difference>
    inline OutputIterator __copy_d(InputIterator start, InputIterator finish, OutputIterator dst,
                                    Difference *){
        Difference len = finish - start;
        for(Difference i = 0; i < len; i++, start++, dst++)
            *dst = *start;
        return dst;
    }

    template <typename T>
    inline T* __copy_t(const T* start, const T* finish, T* dst, _false_type){
        memmove(dst, start, sizeof(T) * (finish - start));
        return dst + (finish - start);
    }

    template <typename T>
    inline T* __copy_t(const T* start, const T* finish, T* dst, _true_type){
        return __copy_d(start, finish, dst, difference_type(start));
    }

    template <typename InputIterator, typename OutputIterator>
    inline OutputIterator __copy(InputIterator start, InputIterator finish, OutputIterator dst,
                                    input_iterator_tag){
        for(; start != finish; ++start, ++dst)
            *dst = *start;
        return dst;
    }

    template <typename InputIterator, typename OutputIterator>
    inline OutputIterator __copy(InputIterator start, InputIterator finish, OutputIterator dst,
                                    random_access_iterator_tag){
        return __copy_d(start, finish, dst, difference_type(start));
    }

    template <typename InputIterator, typename OutputIterator>
    inline OutputIterator _copy(InputIterator start, InputIterator finish, OutputIterator dst) {
        return __copy(start, finish, dst, iterator_category(start));
    }

    template <typename T>
    inline T* _copy(const T* start, const T* finish, T* dst) {
        typedef typename type_traits<T>::has_trivial_assignment_operator htap;
        return __copy_t(start, finish, dst, htap());
    }

    template <typename T>
    inline T* _copy(T* start, T* finish, T* dst) {
        typedef typename type_traits<T>::has_trivial_assignment_operator htap;
        return __copy_t(start, finish, dst, htap());
    }

    template <typename InputIterator, typename OutputIterator>
    inline OutputIterator copy(InputIterator start, InputIterator finish, OutputIterator dst) {
        return _copy(start, finish, dst);
    }

    template <>
    inline char* copy(const char* start, const char* finish, char* dst) {
        memmove(dst, start, finish-start);
        return dst + (finish - start);
    }

    template <>
    inline wchar_t* copy(const wchar_t* start, const wchar_t* finish, wchar_t* dst) {
        memmove(dst, start, finish-start);
        return dst + (finish - start);
    }

    /***************************copy_backward*************************************************/
    template <typename BidirectionalIter1, typename BidirectionalIter2>
    inline BidirectionalIter2 _copy_backward(BidirectionalIter1 first, BidirectionalIter1 last,
                                            BidirectionalIter2 result) {
        while(last != first)
            *--result = *--last;
        return result;
    };

    template <typename BidirectionalIter1, typename BidirectionalIter2>
    inline BidirectionalIter2 _copy_backward_dispatch(BidirectionalIter1 first, BidirectionalIter1 last,
                                                        BidirectionalIter2 result) {
        return _copy_backward(first, last, result);
    };

    template <typename T>
    inline T* _copy_backward_t(const T* first, const T* last, T* result, _true_type) {
        std::ptrdiff_t sz = last - first;
        memmove(result-sz, first, sizeof(T)*sz);
        return result - sz;
    }

    template <typename T>
    inline T* _copy_backward_t(const T* first, const T* last, T* result, _false_type) {
        return _copy_backward(first, last, result);
    }

    template <typename T>
    inline T* _copy_backward_dispatch(T* first, T* last, T* result) {
        typedef typename type_traits<T>::has_trivial_assignment_operator has_trivial;
        return _copy_backward_t(first, last, result, has_trivial());
    }

    template <typename T>
    inline T* _copy_backward_dispatch(const T* first, const T* last, T* result) {
        typedef typename type_traits<T>::has_trivial_assignment_operator has_trivial;
        return _copy_backward_t(first, last, result, has_trivial());
    }

    template <typename BidirectionalIter1, typename BidirectionalIter2>
    inline BidirectionalIter2 copy_backward(BidirectionalIter1 first, BidirectionalIter1 last,
                                            BidirectionalIter2 result) {
        return _copy_backward_dispatch(first, last, result);
    };
}

#endif //TINY_STL_LF_ALGORITHM_H
