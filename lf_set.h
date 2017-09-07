//
// Created by van on 17-9-7.
//

#ifndef TINY_STL_SET_H
#define TINY_STL_SET_H

#include "allocator.h"
#include "lf_function.h"
#include "lf_avl_tree.h"

namespace lf {

    template <typename T, typename Compare = lf::less<T>, typename Alloc = allocator<T> >
    class set {
    public:
        typedef T value_type;
        typedef Compare compare;
        typedef allocator<T> data_allocator;

        typedef avl_tree<value_type, value_type, identity<value_type>,
                Compare, Alloc> container;
        typedef typename container::iterator iterator;
        typedef typename container::const_iterator const_iterator;
        typedef typename container::pointer pointer;
        typedef typename container::const_pointer const_pointer;
        typedef typename container::reference reference;
        typedef typename container::const_reference const_reference;

        typedef typename container::size_type size_type;
        typedef typename container::difference_type difference_type;

    private:
        container cc;

    public:
        set(): cc() {}
        ~set() = default;

        iterator begin() { return cc.begin(); }
        iterator cbegin() const { return cc.cbegin(); }
        iterator end() { return cc.end(); }
        iterator cend() const { return cc.cend(); }

        bool empty() const { return cc.empty(); }
        size_type size() const { return cc.size(); }

        value_type front() { return cc.front(); }
        value_type back() { return cc.back(); }

        void clear() { cc.clear(); }
        void erase(iterator pos) {
            cc.erase(pos);
        }

        void insert(const value_type& x) {
            cc.insert_unique(x);
        }
    };

    template <typename T, typename Compare = lf::less<T>, typename Alloc = allocator<T> >
    class multiset {
    public:
        typedef T value_type;
        typedef Compare compare;
        typedef allocator<T> data_allocator;

        typedef avl_tree<value_type, value_type, identity<value_type>,
                Compare, Alloc> container;
        typedef typename container::iterator iterator;
        typedef typename container::const_iterator const_iterator;
        typedef typename container::pointer pointer;
        typedef typename container::const_pointer const_pointer;
        typedef typename container::reference reference;
        typedef typename container::const_reference const_reference;

        typedef typename container::size_type size_type;
        typedef typename container::difference_type difference_type;

    private:
        container cc;

    public:
        multiset(): cc() {}
        ~multiset() = default;

        iterator begin() { return cc.begin(); }
        iterator cbegin() const { return cc.cbegin(); }
        iterator end() { return cc.end(); }
        iterator cend() const { return cc.cend(); }

        bool empty() const { return cc.empty(); }
        size_type size() const { return cc.size(); }

        value_type front() { return cc.front(); }
        value_type back() { return cc.back(); }

        void clear() { cc.clear(); }
        void erase(iterator pos) {
            cc.erase(pos);
        }

        void insert(const value_type& x) {
            cc.insert_equal(x);
        }
    };
}

#endif //TINY_STL_SET_H
