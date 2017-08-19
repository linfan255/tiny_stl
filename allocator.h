//
// Created by van on 17-8-10.
//

#ifndef TINY_STL_ALLOCATOR_H
#define TINY_STL_ALLOCATOR_H

#include "lf_alloc.h"
#include "lf_construct.h"
#include <new>
#include <cassert>

namespace lf {

    template<typename T>
    class allocator {
    public:
        typedef T               value_type;
        typedef T*              pointer;
        typedef const T*        const_pointer;
        typedef T&              reference;
        typedef const T&        const_reference;
        typedef size_t          size_type;
        typedef std::ptrdiff_t	difference_type;



        static pointer allocate(size_type n = 1);
        static void deallocate(pointer p, size_type n = 1);

        static void construct(pointer p, const_reference value);
        static void destroy(pointer first, pointer last);
        static void construct(pointer p);
        static void destroy(pointer p);
    };

    template<typename T>
    T* allocator<T>::allocate(size_type n) {
        if(!n)	return 0;
        return static_cast<T*>(lf_alloc::allocate(n * sizeof(T)));
    }

    template<typename T>
    void allocator<T>::deallocate(pointer p, size_type n) {
        if(!n)	return;
        lf_alloc::deallocate(static_cast<void*>(p), n * sizeof(T));
    }

    template<typename T>
    void allocator<T>::construct(pointer p, const_reference value) {
        new(p) T(value);
    }

    template<typename T>
    void allocator<T>::construct(pointer p) {
        new(p) T();
    }

    template<typename T>
    void allocator<T>::destroy(pointer first, pointer last) {
        for(; first != last; ++first)
            destroy(first);
    }

    template<typename T>
    void allocator<T>::destroy(pointer p) {
        p->~T();
    }
}


#endif //TINY_STL_ALLOCATOR_H
