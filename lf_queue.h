//
// Created by van on 17-8-24.
//

#ifndef TINY_STL_LF_QUEUE_H
#define TINY_STL_LF_QUEUE_H

#include "lf_deque.h"

namespace lf {
    template <typename T, typename Container = deque<T>>
    class queue {
    public:
        typedef typename Container::value_type      value_type;
        typedef typename Container::pointer         pointer;
        typedef typename Container::reference       reference;
        typedef typename Container::const_pointer   const_pointer;
        typedef typename Container::const_reference const_reference;
        typedef typename Container::difference_type difference_type;
        typedef typename Container::size_type       size_type;

    private:
        Container cc;

    public:
        queue() = default;
        queue(size_type n, const value_type& val): cc(n, val) {}
        queue(const queue& other): cc(other.cc) {}
        ~queue() = default;

        queue& operator=(const queue& rhs);

        void pop() { cc.pop_front(); }
        void push(const value_type& x) { cc.push_back(x); }

        bool empty() const { return cc.empty(); }
        size_type size() const { return cc.size(); }

        reference front() { return cc.front(); }
        const_reference front() const { cc.front(); }
        reference back() { return cc.back(); }
        const_reference back() const { return cc.back(); }
    };

    template <typename T, typename Container>
    queue<T,Container>& operator=(const queue<T,Container>& rhs) {
        cc = rhs.cc;
    }
}

#endif //TINY_STL_LF_QUEUE_H
