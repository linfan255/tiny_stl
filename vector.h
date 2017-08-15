//
// Created by van on 17-8-13.
//

#ifndef TINY_STL_VECTOR_H
#define TINY_STL_VECTOR_H

#include "allocator.h"
#include "lf_uninitialized.h"
#include "lf_algorithm.h"

namespace lf {

    template <typename T, typename Alloc = allocator<T> >
    class vector {
    public:
        typedef T                   value_type;
        typedef value_type*         pointer;
        typedef const value_type*   const_pointer;
        typedef value_type&         reference;
        typedef const value_type&   const_reference;
        typedef std::ptrdiff_t      difference_type;
        typedef size_t              size_type;
        typedef value_type*         iterator;
        typedef const value_type*   const_iterator;
        typedef allocator<T>        data_allocator;

    private:
        iterator start;
        iterator finish;
        iterator end_of_cap;    //第一个存储元素，最后一个存储元素，存储空间的末尾


    public:
        vector(): start(nullptr), finish(nullptr), end_of_cap(nullptr) {}
        vector(const size_type n, const value_type & val);
        explicit vector(size_type n);
        //template <typename InputIterator> vector(InputIterator s, InputIterator t);
        vector(const vector& other);
        vector(vector&& other);
        ~vector();

        //赋值运算符
        vector& operator=(const vector& rhs);

        //功能函数
        iterator begin() { return start; }
        const_iterator begin() const { return start; }
        const_iterator cbegin() const { return start; }

        iterator end() { return finish; }
        const_iterator end() const { return finish; }
        const_iterator cend() const { return finish; }

        size_type size() const { return (size_type)(end() - begin()); }
        size_type capacity() const { return (size_type)(end_of_cap - begin()); };
        bool empty() const { return begin() == end(); }
        reference operator[](size_type i) { return *(start + i); }
        const_reference operator[](size_type i) const { return *(start + i); }
        reference front() { return *start; }
        reference back() { return *(finish -1); }
        void clear();

        void insert(iterator pos, const T& val);    //在某一个位置上插入元素
        void insert(iterator pos, size_type n, const T& val);
        void push_back(const T& val);
        void pop_back();
        void erase(iterator pos);
        void erase(iterator first, iterator last);

        void resize(size_type new_size);
        void resize(size_type new_size, const value_type& val);
    };


    template<typename T, typename Alloc>
    void vector<T, Alloc>::clear() {
        data_allocator::destroy(begin(), end());
        finish = start;
    }

    template <typename T, typename Alloc>
    vector<T, Alloc>::vector(const vector::size_type n, const value_type &val) {
        start = data_allocator::allocate(n);
        uninitialized_fill_n(start, n, val);
        finish = start + n;
        end_of_cap = finish;
    }

    template <typename T, typename Alloc>
    vector<T, Alloc>::vector(vector::size_type n) {
        start = data_allocator::allocate(n);
        uninitialized_fill_n(start, n, T());
        finish = end_of_cap = start + n;
    }
/*
    template<typename T, typename Alloc>
    template<typename InputIterator>
    vector<T, Alloc>::vector(InputIterator s, InputIterator t) {
        start = data_allocator::allocate(t - s);
        finish = uninitialized_copy(s, t, start);
        end_of_cap = start + (t - s);
    }
*/
    template<typename T, typename Alloc>
    vector<T, Alloc>::vector(const vector &other) {
        start = data_allocator::allocate(other.finish - other.start);
        finish = uninitialized_copy(other.start, other.finish, start);
        end_of_cap = start + (other.finish - other.start);
    }

    template<typename T, typename Alloc>
    vector<T, Alloc>::vector(vector &&other) {
        start = other.start;
        finish = other.finish;
        end_of_cap = other.end_of_cap;
        other.start = other.finish = other.end_of_cap = nullptr;
    }

    template<typename T, typename Alloc>
    vector<T, Alloc> &vector<T, Alloc>::operator=(const vector &rhs) {
        if(this == &rhs)    return *this;

        size_type rhs_len = rhs.size();
        if(rhs_len > capacity()) {  //如果另外一个vector的现有元素数量多出当前vector的容量
            iterator new_start = data_allocator::allocate(rhs_len);
            iterator new_finish = uninitialized_copy(rhs.begin(), rhs.end(), new_start);

            destroy(begin(), end());
            data_allocator::deallocate(begin(), capacity());

            start = new_start;
            finish = new_finish;
            end_of_cap = start + rhs_len;
        }
        else if(size() > rhs_len) { //如果当前容器元素数量大于rhs容器中的数量
            iterator new_finish = copy(rhs.begin(), rhs.end(), begin());
            destroy(new_finish, finish);
            finish = new_finish;
        }
        else {  //如果当前容器中的元素数量小于rhs
            copy(rhs.begin(), rhs.begin() + size(), begin());
            uninitialized_copy(rhs.begin() + size(), rhs.end(), end());
            finish = begin() + rhs_len;
        }
        return *this;
    }

    template<typename T, typename Alloc>
    void vector<T, Alloc>::insert(vector::iterator pos, const T &val) {
        if(end() == end_of_cap) {
            //如果容量不足，需要扩容
            size_type new_cap = capacity() == 0 ? 1 : 2 * capacity();

            iterator new_start = data_allocator::allocate(new_cap);
            iterator new_finish = new_start;
            try {
                new_finish = uninitialized_copy(start, pos, new_start);
                construct(new_finish, val);
                new_finish++;
                new_finish = uninitialized_copy(pos, finish, new_finish);
            } catch(...) {
                destroy(new_start, new_finish);
                data_allocator::deallocate(new_start, new_cap);
                throw;
            }

            destroy(begin(), end());
            data_allocator::deallocate(begin(), capacity());
            start = new_start;
            finish = new_finish;
            end_of_cap = start + new_cap;
        }
        else {
            //如果容量足够
            construct(finish++, back());
            std::copy_backward(pos, finish-2, finish-1);    //将[pos,finish)的元素整体后移
            *pos = val;
        }
    }

    template<typename T, typename Alloc>
    void vector<T, Alloc>::insert(iterator pos, size_type n, const T &val) {
        if(!n)  return;

        if(n > end_of_cap - end()) {
            //如果容量不足，需要扩容
            size_type new_size = capacity() + (n > capacity() ? n : capacity());
            iterator new_start = data_allocator::allocate(new_size);
            iterator new_finish = new_start;

            new_finish = uninitialized_copy(begin(), pos, new_start);
            new_finish = uninitialized_fill_n(new_finish, n, val);
            new_finish = uninitialized_copy(pos, end(), new_finish);


            destroy(begin(), end());
            data_allocator::deallocate(begin(), capacity());

            start = new_start;
            finish = new_finish;
            end_of_cap = start + new_size;
        } else {
            //如果空间容量足够
            std::copy_backward(pos, end(), end() + n);
            finish += n;

            iterator p = pos;
            for(int i = 0; i < n; i++)
                *(p + i) = val;
        }
    }

    template<typename T, typename Alloc>
    void vector<T, Alloc>::push_back(const T &val) {
        if(end() != end_of_cap) {
            //如果任然有足够的空间
            construct(finish++, val);
        } else {
            insert(end(), val);
        }
    }


    template<typename T, typename Alloc>
    vector<T, Alloc>::~vector() {
        destroy(start, finish);
        data_allocator::deallocate(start, capacity());
    }

    template<typename T, typename Alloc>
    void vector<T, Alloc>::pop_back() {
        finish--;
        destroy(finish);
    }

    template<typename T, typename Alloc>
    void vector<T, Alloc>::erase(vector::iterator pos) {
        if(pos != end()) {
            if(pos + 1 == end()) {
                pop_back();
            } else {
                copy(pos + 1, finish, pos);
                pop_back();
            }
        }
    }

    template<typename T, typename Alloc>
    void vector<T, Alloc>::erase(vector::iterator first, vector::iterator last) {
        if(first < begin() || last > end())    return;

        iterator new_finish = copy(last, finish, first);
        destroy(new_finish, finish);
        finish = new_finish;
    }

    template<typename T, typename Alloc>
    void vector<T, Alloc>::resize(vector::size_type new_size, const vector::value_type& val) {
        if(new_size < size()) {
            erase(start + new_size, end());
        } else {
            insert(end(), new_size - size(), val);
        }
    }

    template<typename T, typename Alloc>
    void vector<T, Alloc>::resize(size_type new_size) {
        resize(new_size, T());
    }
}

#endif //TINY_STL_VECTOR_H
