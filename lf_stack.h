//
// Created by van on 17-8-24.
//

#ifndef TINY_STL_LF_STACK_H
#define TINY_STL_LF_STACK_H

#include "allocator.h"
#include "lf_deque.h"

namespace lf {

    template <typename T, typename Container = deque<T> >
    class stack {
    public:
        typedef typename Container::value_type      value_type;
        typedef typename Container::pointer         pointer;
        typedef typename Container::reference       reference;
        typedef typename Container::const_pointer   const_pointer;
        typedef typename Container::const_reference const_reference;
        typedef typename Container::size_type       size_type;
        typedef typename Container::difference_type difference_type;

    private:
        Container cc;

    public:
        stack() = default;
        stack(const stack& other);
        stack(size_type n, const value_type& val);
        ~stack() = default;

        stack& operator=(const stack& rhs);

        void push(const value_type& val) { cc.push_back(val); }
        void pop() { cc.pop_back(); }

        size_type size() const { return cc.size(); }
        bool empty() const { return cc.empty(); }

        reference top() { return cc.back(); }
        const_reference top() const { return cc.back(); }
    };

    template <typename T, typename Container>
    stack<T,Container>::stack(size_type n, const value_type &val): cc(n, val) {}

    template <typename T, typename Container>
    stack<T,Container>::stack(const stack &other): cc(other.cc) {}

    template <typename T, typename Container>
    stack<T,Container>& stack<T,Container>::operator=(const stack& rhs) {
        cc = rhs.cc;
    }
}

#endif //TINY_STL_LF_STACK_H
