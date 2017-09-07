//
// Created by van on 17-9-7.
//

#ifndef TINY_STL_LF_MAP_H
#define TINY_STL_LF_MAP_H

#include "lf_function.h"
#include "allocator.h"
#include "lf_avl_tree.h"
#include "lf_pair.h"

namespace lf {

    template <typename Key, typename Value, typename Compare = lf::less<Value>, typename Alloc = allocator<Value> >
    class map {
    public:
        typedef Key key_type;
        typedef lf::pair<key_type, Value> value_type;
        typedef Compare compare;
        typedef allocator<value_type> data_allocator;

        typedef avl_tree<key_type, value_type, lf::getPairKey<value_type>,
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
        map(): cc() {}
        ~map() = default;

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

    template <typename Key, typename Value, typename Compare = lf::less<Value>, typename Alloc = allocator<Value> >
    class multimap {
    public:
        typedef Key key_type;
        typedef lf::pair<key_type, Value> value_type;
        typedef Compare compare;
        typedef allocator<value_type> data_allocator;

        typedef avl_tree<key_type, value_type, lf::getPairKey<value_type>,
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
        multimap(): cc() {}
        ~multimap() = default;

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

#endif //TINY_STL_LF_MAP_H
