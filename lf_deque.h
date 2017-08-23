//
// Created by van on 17-8-20.
//

#ifndef TINY_STL_LF_DEQUE_H
#define TINY_STL_LF_DEQUE_H

#include <cstddef>
#include "lf_iterator.h"
#include "allocator.h"
#include "lf_uninitialized.h"
#include "lf_algorithm.h"

namespace lf {
    static inline size_t get_buffer_size(size_t BufferSize, size_t sz) {
        if(BufferSize)
            return BufferSize;
        else {
            if(sz < 512)
                return 512 / sz;
            else
                return 1;
        }
    }

    template<typename T, typename Ptr, typename Ref, size_t BufferSize = 0>
    struct _deque_iterator: public iterator<random_access_iterator_tag, T> {
        using typename iterator<random_access_iterator_tag,T>::difference_type;
        using typename iterator<random_access_iterator_tag,T>::pointer;
        using typename iterator<random_access_iterator_tag,T>::reference;
        using typename iterator<random_access_iterator_tag,T>::value_type;
        using typename iterator<random_access_iterator_tag,T>::iterator_category;
        using typename iterator<random_access_iterator_tag,T>::const_pointer;
        using typename iterator<random_access_iterator_tag,T>::const_reference;

        typedef _deque_iterator<T, T*, T&, BufferSize>              iterator;
        typedef _deque_iterator<T, const T*, const T&, BufferSize>  const_iterator;
        typedef pointer*                                            map_pointer;
        typedef size_t                                              size_type;
        typedef _deque_iterator                                     self;


        pointer first;
        pointer last;
        pointer curr;
        map_pointer map_ptr;        //分别是缓冲区中的头、尾、所指元素，以及指向控制块的指针

        //计算每个缓冲区最多能够存放多少单位的元素
        size_t buffer_size() const {
            return get_buffer_size(BufferSize, sizeof(value_type));
        }

        //ctor & dtor
        _deque_iterator() = default;
        _deque_iterator(const iterator& other): first(other.first), last(other.last),
                                                curr(other.curr), map_ptr(other.map_ptr) {}
        ~_deque_iterator() = default;

        iterator& operator=(const iterator& rhs) {
            if(this == &rhs)    return *this;
            first = rhs.first;
            last = rhs.last;
            curr = rhs.curr;
            map_ptr = rhs.map_ptr;
        }

        void switch_map(map_pointer new_map_ptr) {
            map_ptr = new_map_ptr;
            first = *new_map_ptr;
            last = first + buffer_size();
        }

        const_reference operator* () const { return *curr; }
        const_pointer operator->() const { return &(operator*()); }
        reference operator* () { return *curr; }
        pointer operator->() { return &(operator*()); }

        difference_type operator-(const self& rhs) const {
            return (difference_type)(buffer_size()) * (map_ptr - rhs.map_ptr - 1) +
                    (curr - first) + (rhs.last - rhs.curr);
        }


        //前置递增
        iterator& operator++() {
            ++curr;
            if(curr == last) {
                switch_map(map_ptr + 1);
                curr = first;
            }
            return *this;
        }

        //后置递增
        self operator++(int) {
            self ret = *this;
            ++curr;
            if(curr == last) {
                switch_map(map_ptr + 1);
                curr = first;
            }
            return ret;
        }

        //递减
        iterator& operator--() {
            if(curr == first) {
                switch_map(map_ptr - 1);
                curr = last;
            }
            --curr;
            return *this;
        }
        iterator operator--(int) {
            iterator ret = *this;
            if(curr == first) {
                switch_map(map_ptr - 1);
                curr = last;
            }
            --curr;
            return ret;
        }


        iterator& operator+=(difference_type x) {
            difference_type offset = x + (curr - first);    //求出相对于first的偏移量
            if(offset >= 0 && buffer_size() > offset)
                curr += offset;
            else {
                //如果不在同一个缓冲区中
                difference_type map_offset = offset > 0 ? (offset / buffer_size()) :
                                             -((-offset - 1) / buffer_size()) - 1;
                switch_map(map_ptr + map_offset);
                curr = first + (offset - buffer_size() * map_offset);
            }
            return *this;
        }

        iterator& operator-=(difference_type x) {
            return *this += -x;
        }

        iterator& operator+(difference_type x) {
            iterator tmp = *this;
            return tmp += x;
        }

        iterator& operator-(difference_type x) {
            iterator tmp = *this;
            return tmp -= x;
        }

        reference operator[](size_type n) {
            return *(*this + n);
        }
        const_reference operator[](size_type n) const {
            return *(*this + n);
        }

        bool operator==(const iterator& rhs) const {
            return curr == rhs.curr;
        }

        bool operator!=(const iterator& rhs) const {
            return curr != rhs.curr;
        }

        bool operator<(const iterator& rhs) const {
            return map_ptr == rhs.map_ptr ? curr < rhs.curr : map_ptr < rhs.map_ptr;
        }
    };


    template <typename T, typename Alloc = allocator<T>, size_t BufferSize = 0 >
    class deque {
    public:
        typedef T               value_type;
        typedef T*              pointer;
        typedef T&              reference;
        typedef const T*        const_pointer;
        typedef const T&        const_reference;
        typedef size_t          size_type;
        typedef ptrdiff_t       difference_type;

        typedef _deque_iterator<T,T*,T&,BufferSize>             iterator;
        typedef _deque_iterator<T,const T*,const T&,BufferSize> const_iterator;

        typedef pointer*            map_pointer;
        typedef allocator<T>        data_allocator;
        typedef allocator<T*>       map_allocator;

    private:
        iterator start;
        iterator finish;
        map_pointer map;
        size_type map_size; //指向头尾的迭代器、映射表以及映射表的长度

    public:
        deque(): start(), finish(), map(0), map_size(0) {allocate_map(0);}
        deque(size_type n, const value_type& val);
        deque(const deque& other);
        ~deque();

        deque& operator=(const deque& rhs);     //ctor & dtor

    public:
        iterator begin() { return start; }
        iterator end() { return finish; }
        const_iterator begin() const { return start; }
        const_iterator end() const { return finish; }

        reference operator[](size_type i) { return start[i]; }
        const_reference operator[](size_type i) const { return start[i]; }

        reference front() { return *start; }
        const_reference front() const { return *start; }
        reference back();
        const_reference back() const;
        size_type size() const { return finish - start; }
        bool empty() const { return start == finish; }

        void push_back(const value_type& val);
        void push_front(const value_type& val);
        void pop_back();
        void pop_front();
        void clear();
        iterator erase(iterator pos);
        iterator erase(iterator first, iterator last);
        iterator insert(iterator pos, const value_type& val);

    protected:
        //一些辅助性的函数
        size_t buffer_size() const { return get_buffer_size(BufferSize, sizeof(value_type)); }
        void allocate_map(size_type n);
        void reallocate_map(bool add_front);
    };


    template <typename T, typename Alloc, size_t BufSize>
    void deque<T,Alloc,BufSize>::reallocate_map(bool add_front) {
        size_type old_map_num = finish.map_ptr - start.map_ptr + 1;
        size_type new_map_num = old_map_num + 1;
        map_pointer new_start, new_finish;

        if(new_map_num * 2 < map_size) {
            //现有的map空间足够大，只需要调整位置即可
            new_start = map + (map_size - new_map_num) / 2 + (add_front ? 1 : 0);
            new_finish = new_start + old_map_num - 1;

            if(new_start < start.map_ptr) {
                copy(start.map_ptr, finish.map_ptr+1, new_start);
            } else {
                copy_backward(start.map_ptr, finish.map_ptr+1, new_finish+1);
            }
        } else {
            //现有的map空间过小，需要重新分配一块足够大的空间
            size_type new_map_size = 2 * map_size + 2;
            map_pointer new_map = map_allocator::allocate(new_map_size);
            new_start = new_map + (new_map_size - new_map_num) / 2 + (add_front ? 1 : 0);
            new_finish = new_start + old_map_num - 1;

            copy(start.map_ptr, finish.map_ptr+1, new_start);
            map_allocator::deallocate(map, map_size);

            map = new_map;
            map_size = new_map_size;
        }

        start.switch_map(new_start);
        finish.switch_map(new_finish);
    }

    template <typename T, typename Alloc, size_t BufSize>
    void deque<T,Alloc,BufSize>::push_front(const value_type &val) {
        if(start.curr >= start.first + 1) {
            construct(--start.curr, val);
        } else {
            if(start.map_ptr == map + 1)
                reallocate_map(true);
            *(start.map_ptr - 1) = data_allocator::allocate(buffer_size());
            start.switch_map(start.map_ptr - 1);
            start.curr = start.last - 1;
            construct(start.curr, val);
        }
    }

    template <typename T, typename Alloc, size_t BufSize>
    void deque<T,Alloc,BufSize>::push_back(const value_type& val) {
        if(finish.curr < finish.last-1) { //如果在最后一个缓冲区中还有一个以上的空闲区块j
            construct(finish.curr++, val);
        } else {    //如果只剩下了一个空闲块的话，需要将finish移动到下一个缓冲区
            if(finish.map_ptr == map + map_size - 1) {
                //如果下一个缓冲区是最后一个的话，需要重新调整map
                reallocate_map(false);
            }
            *(finish.map_ptr + 1) = data_allocator::allocate(buffer_size());    //分配好下一个缓冲区
            construct(finish.curr, val);
            finish.switch_map(finish.map_ptr + 1);
            finish.curr = finish.first;
        }
    }

    template <typename T, typename Alloc, size_t BufSize>
    void deque<T,Alloc,BufSize>::allocate_map(size_type n) {
        size_type map_num = n / buffer_size() + 1;
        map_size = (8 > map_num + 2 ? 8 : map_num + 2); //最少分配8个map

        map = map_allocator::allocate(map_size);

        map_pointer map_start = map + (map_size - map_num) / 2;
        map_pointer map_finish = map_start + map_num - 1;
        map_pointer cur;

        try {
            for(cur = map_start; cur <= map_finish; ++cur) {
                *cur = data_allocator::allocate(buffer_size());
            }
        } catch (...) {
            for(map_pointer p = map_start; p < cur; ++p)
                data_allocator::deallocate(*cur, buffer_size());
        }

        start.switch_map(map_start);
        finish.switch_map(map_finish);

        start.curr = start.first;
        finish.curr = finish.first + (n % buffer_size());
    }

    template <typename T, typename Alloc, size_t BufSize>
    T& deque<T,Alloc,BufSize>::back() {
        iterator ret = finish;
        --ret;
        return *ret;
    }

    template <typename T, typename Alloc, size_t BufSize>
    const T& deque<T,Alloc,BufSize>::back() const {
        iterator ret = finish;
        --ret;
        return *ret;
    }

    template <typename T, typename Alloc, size_t BufSize>
    deque<T,Alloc,BufSize>::deque(deque::size_type n, const value_type &val) {
        allocate_map(n);

        map_pointer curr;
        try {
            for(curr = start.map_ptr; curr < finish.map_ptr; ++curr)
                uninitialized_fill_n(*curr, buffer_size(), val);
            uninitialized_fill(finish.first, finish.curr, val);
        } catch(...) {
            for(map_pointer mp = start.map_ptr; mp <= finish.map_ptr; ++mp) {
                data_allocator::deallocate(*mp, buffer_size());
            }
            map_allocator::deallocate(map, map_size);
            throw;
        }
    }


    template <typename T, typename Alloc, size_t BufSize>
    deque<T,Alloc,BufSize>::~deque() {
        destroy(begin(), end());
        for(map_pointer mp = start.map_ptr; mp <= finish.map_ptr; ++mp)
            data_allocator::deallocate(*mp, buffer_size());
        map_allocator::deallocate(map, map_size);
    }

    template <typename T, typename Alloc, size_t BufSize>
    deque<T,Alloc,BufSize>::deque(const deque &other) {
        allocate_map(other.size());
        uninitialized_copy(other.begin(), other.end(), begin());
    }

    template <typename T, typename Alloc, size_t BufSize>
    deque<T,Alloc,BufSize> &deque<T,Alloc,BufSize>::operator=(const deque &rhs) {
        if(this == &rhs)
            return *this;

        if(size() > rhs.size()) {
            iterator pos = copy(rhs.begin(), rhs.end(), begin());
            erase(pos, end());
        } else {
            size_type len = size();
            copy(rhs.begin(), rhs.begin() + len, begin());
            for(auto it = rhs.begin() + len; it != rhs.end(); ++it)
                push_back(*it);
        }
    }

    template <typename T, typename Alloc, size_t BufSize>
    typename deque<T,Alloc,BufSize>::iterator
    deque<T,Alloc,BufSize>::insert(iterator pos, const value_type &val) {

    }
}

#endif //TINY_STL_LF_DEQUE_H
