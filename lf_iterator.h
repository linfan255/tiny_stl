//
// Created by van on 17-8-10.
//

#ifndef TINY_STL_LF_ITERATOR_H_H
#define TINY_STL_LF_ITERATOR_H_H

#include <cstddef>

namespace lf {

    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_terator_tag: public input_iterator_tag {};
    struct bidirectional_iterator_tag: public forward_terator_tag {};
    struct random_access_iterator_tag: public bidirectional_iterator_tag {};

    //所有迭代器的父类
    template <typename Category, typename T, typename Distance = std::ptrdiff_t,
            typename Pointer = T*, typename Reference = T& > struct iterator {
        typedef Category    iterator_category;
        typedef T           value_type;
        typedef Distance    difference_type;
        typedef Pointer     pointer;
        typedef Reference   reference;
        typedef const Pointer const_pointer;
        typedef const Reference const_reference;
    };

    //traits
    template <typename Iterator>
    struct iterator_traits {
        typedef typename Iterator::iterator_category    iterator_category;
        typedef typename Iterator::T                    value_type;
        typedef typename Iterator::Distance             difference_type;
        typedef typename Iterator::Pointer              pointer;
        typedef typename Iterator::reference            reference;
    };

    //针对指针的两个偏特化版本
    template <typename T>
    struct iterator_traits<T*> {
        typedef random_access_iterator_tag  iterator_category;
        typedef T                           value_type;
        typedef std::ptrdiff_t              difference_type;
        typedef T*                          pointer;
        typedef T&                          reference;
    };

    template <typename T>
    struct iterator_traits<const T*> {
        typedef random_access_iterator_tag  iterator_category;
        typedef T                           value_type;
        typedef std::ptrdiff_t              difference_type;
        typedef T*                          pointer;
        typedef T&                          reference;
    };

    //该函数能够求出迭代器的value_type;
    template <typename Iterator>
    inline typename iterator_traits<Iterator>::value_type*
    value_type(const Iterator& iter) {
        return static_cast<typename iterator_traits<Iterator>::pointer>(0);
    }

    //求出迭代器的difference type
    template <typename Iterator>
    inline typename iterator_traits<Iterator>::difference_type*
    difference_type(const Iterator& iter) {
        return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
    }

    //求出迭代器的category
    template <typename Iterator>
    inline typename iterator_traits<Iterator>::iterator_category
    iterator_category(const Iterator& iter) {
        typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
        return iterator_category();
    }

}


#endif //TINY_STL_LF_ITERATOR_H_H
