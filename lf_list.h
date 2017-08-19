//
// Created by van on 17-8-15.
//

#ifndef TINY_STL_LF_LIST_H
#define TINY_STL_LF_LIST_H

#include "allocator.h"
#include "lf_iterator.h"

namespace lf{

    template<typename T>
    struct _list_node {
        T data;
        _list_node* prev;
        _list_node* next;

        _list_node() = default;
        explicit _list_node(T _data): data(_data), prev(nullptr), next(nullptr) {}
    };  //链表的节点结构

    template<typename T>
    struct _list_iterator: public iterator<bidirectional_iterator_tag,T> {
        typedef _list_iterator<T> list_iterator;
        typedef _list_node<T>* node_ptr;

        node_ptr ptr;   //真正指向_list_node的指针

        //ctor
        _list_iterator() = default;
        _list_iterator(node_ptr _ptr): ptr(_ptr) {}
        _list_iterator(const list_iterator& other): ptr(other.ptr) {}

        ~_list_iterator() = default;

        list_iterator& operator=(const list_iterator& rhs) {
            ptr = rhs.ptr;
            return *this;
        }

        typename iterator<bidirectional_iterator_tag,T>::const_reference
        operator*() const { return ptr->data; }

        typename iterator<bidirectional_iterator_tag,T>::reference
        operator*() { return ptr->data; }

        typename iterator<bidirectional_iterator_tag,T>::const_pointer
        operator->() const { return &(operator*()); }

        typename iterator<bidirectional_iterator_tag,T>::pointer
        operator->() { return &(operator*()); }

        list_iterator& operator++() {
            ptr = ptr->next;
            return *this;
        }

        list_iterator operator++(int) {
            //后置类型
            list_iterator ret = *this;
            ptr = ptr->next;
            return ret;
        }

        list_iterator& operator--() {
            ptr = ptr->prev;
            return *this;
        }

        list_iterator operator--(int) {
            list_iterator ret = *this;
            ptr = ptr->prev;
            return ret;
        }

        bool operator==(const list_iterator& rhs) const {
            return ptr == rhs.ptr;
        }

        bool operator!=(const list_iterator& rhs) const {
            return ptr != rhs.ptr;
        }
    };


    template<typename T, typename Alloc=allocator<_list_node<T> > >
    class list {
    public:
        typedef T                   value_type;
        typedef value_type*         pointer;
        typedef const value_type*   const_pointer;
        typedef value_type&         reference;
        typedef const value_type&   const_reference;
        typedef size_t              size_type;

        typedef _list_iterator<T>           iterator;
        typedef const _list_iterator<T>     const_iterator;
        typedef _list_node<T>               node_type;
        typedef _list_node<T>*              link_type;
        typedef allocator<_list_node<T>>    data_allocator;

    private:
        iterator header;

        void transfer(iterator pos, iterator first, iterator last);

        //申请和释放一个节点的容量
        link_type alloc_node() { return data_allocator::allocate(); }
        void dealloc_node(link_type p) { data_allocator::deallocate(p); }

        //从内存池中分配一个节点的内存容量，并同时构造节点，之后返回指针
        link_type construct_node(const T& x) {
            link_type ret = alloc_node();
            construct(&ret->data, x);
            return ret;
        }

        //将节点的内存归还给内存池
        void destroy_node(link_type p) {
            destroy(&p->data);
            dealloc_node(p);
        }

    public:
        list();
        explicit list(size_type n, value_type val = value_type());
        ~list();
        list(const list& other);
        list& operator=(const list& rhs);

        iterator begin() { return header.ptr->next; }
        iterator end()   { return header; }
        const_iterator begin() const { return header.ptr->next; }
        const_iterator end() const { return header; }

        void clear();
        void push_back(value_type x);
        void pop_back();
        void push_front(value_type x);
        void pop_front();

        bool empty() const { return header.ptr->next == header.ptr; }
        typename iterator::difference_type size() const;
        reference front() { header.ptr->next->data; }
        reference back() { header.ptr->prev->data; }
        const_reference front() const { header.ptr->next->data; };
        const_reference back() const { header.ptr->prev->data; }

        iterator insert(iterator pos, value_type x);
        void insert(iterator pos, size_type n, value_type x);
        iterator erase(iterator pos);
        void erase(iterator first, iterator last);

        void swap(list& other);
        void remove(const value_type& x);
        void unique();
        void splice(iterator pos, list& other);
        void splice(iterator pos, list& other, iterator it);
        void splice(iterator pos, list& other, iterator first, iterator last);

        void merge(list& other);
        void reverse();
        void sort();
    };

    template <typename T, typename Alloc>
    list<T, Alloc>::list(list::size_type n, value_type val) {
        insert(end(),n,val);
    }

    template<typename T, typename Alloc>
    list<T,Alloc>::list() {
        header = construct_node(T());
        header.ptr->next = header.ptr;
        header.ptr->prev = header.ptr;
    }

    template<typename T, typename Alloc>
    list<T,Alloc>::list(const list &other) {
        header = construct_node(T());
        for(auto it = other.begin(); it != other.end(); it++)
            push_back(*it);
    }

    /**在某个位置后面插入n个x*/
    template<typename T, typename Alloc>
    void list<T,Alloc>::insert(list::iterator pos, size_t n, value_type x) {
        iterator prev_iterator = --pos, tmp = pos;
        iterator next_iterator = ++pos;

        for(int i = 0; i < n; i++) {
            iterator it = construct_node(x);
            tmp.ptr->next = it.ptr;
            it.ptr->prev = tmp.ptr;
            tmp++;
        }

        tmp.ptr->next = next_iterator.ptr;
        next_iterator.ptr->prev = tmp.ptr;
    }

    template<typename T, typename Alloc>
    typename list<T,Alloc>::iterator list<T, Alloc>::insert(iterator pos, value_type x) {
        iterator it = construct_node(x);
        it.ptr->next = pos.ptr;
        it.ptr->prev = pos.ptr->prev;
        pos.ptr->prev->next = it.ptr;
        pos.ptr->prev = it.ptr;
        return it;
    }

    /*将区间[first,last)中的所有元素移动到pos之前*/
    template<typename T, typename Alloc>
    void list<T,Alloc>::transfer(list::iterator pos, list::iterator first, list::iterator last) {
        if(pos == last)
            return;

        last.ptr->prev->next = pos.ptr;
        first.ptr->prev->next = last.ptr;
        pos.ptr->prev->next = first.ptr;
        link_type tmp = pos.ptr->prev;

        pos.ptr->prev = last.ptr->prev;
        last.ptr->prev = first.ptr->prev;
        first.ptr->prev = tmp;
    }

    template<typename T, typename Alloc>
    list<T,Alloc> &list<T,Alloc>::operator=(const list &rhs) {
        header = construct_node(T());
        for(auto it = rhs.begin(); it != rhs.end(); ++it)
            push_back(*it);
        return *this;
    }

    template<typename T, typename Alloc>
    void list<T,Alloc>::clear() {
        iterator it = begin();
        while(it != end()) {
            iterator tmp = it++;
            dealloc_node(tmp.ptr);
        }
        header.ptr->next = header.ptr;
        header.ptr->prev = header.ptr;
    }

    template<typename T, typename Alloc>
    list<T, Alloc>::~list() {
        //将所有的分配内存归还给内存池
        for(iterator it = begin(); it != end(); ++it)
            dealloc_node(it.ptr);
    }

    template<typename T, typename Alloc>
    void list<T,Alloc>::push_back(value_type x) {
        insert(end(),x);
    }

    template<typename T, typename Alloc>
    void list<T, Alloc>::pop_back() {
        iterator it = end();
        erase(--it);
    }

    template<typename T, typename Alloc>
    void list<T, Alloc>::push_front(value_type x) {
        insert(begin(), x);
    }

    template<typename T, typename Alloc>
    void list<T, Alloc>::pop_front() {
        erase(begin());
    }

    template<typename T, typename Alloc>
    typename _list_iterator<T>::difference_type
    list<T, Alloc>::size() const {
        typename iterator::difference_type len = 0;
        for(iterator it = begin(); it != end(); it++)
            len++;
        return len;
    }

    template<typename T, typename Alloc>
    typename list<T,Alloc>::iterator list<T,Alloc>::erase(iterator pos) {
        iterator ret = pos.ptr->next;
        pos.ptr->prev->next = pos.ptr->next;
        pos.ptr->next->prev = pos.ptr->prev;
        dealloc_node(pos.ptr);
        return ret;
    }

    template<typename T, typename Alloc>
    void list<T, Alloc>::erase(iterator first, iterator last) {
        for(iterator it = first; it != last; it++)
            erase(it);
    }

    template<typename T, typename Alloc>
    void list<T, Alloc>::swap(list &other) {
        iterator tmp = header;
        header = other.header;
        other.header = tmp;
    }

    //将所有值为x的元素都移除
    template<typename T, typename Alloc>
    void list<T, Alloc>::remove(const value_type &x) {
        iterator p = begin();
        while(p != end()) {
            iterator tmp = p++;
            if(*tmp == x)
                erase(tmp);
        }
    }

    template<typename T, typename Alloc>
    void list<T, Alloc>::unique() {
        iterator left = begin();
        iterator right = left;
        if(left == end())
            return;

        while(++right != end()) {
            if(*right == *left)
                erase(right);
            else
                left = right;
            right = left;
        }
    }

    //将链表other链接到本链表中pos位置之前
    template<typename T, typename Alloc>
    void list<T, Alloc>::splice(list::iterator pos, list &other) {
        if(other.empty())
            return;
        transfer(pos, other.begin(), other.end());
    }

    template<typename T, typename Alloc>
    void list<T, Alloc>::splice(list::iterator pos, list &other, list::iterator it) {
        iterator left = it;
        iterator right = left;
        right++;

        if(pos == left || pos == right)
            return;

        transfer(pos, left, right);
    }

    template<typename T, typename Alloc>
    void list<T,Alloc>::splice(iterator pos, list &other, iterator first, iterator last) {
        if(first != last && pos != last)
            transfer(pos, first, last);
    }

    template<typename T, typename Alloc>
    void list<T,Alloc>::merge(list &other) {
        iterator it1 = begin(), it2 = other.begin();

        while(it2 != other.end() && it1 != end()) {
            if(*it1 > *it2) {
                iterator tmp = it2++;
                splice(it1, other, tmp);
            } else {
                it1++;
            }
        }

        if(it2 != other.end())
            splice(end(), other, it2, other.end());
    }

    //逆转链表
    template<typename T, typename Alloc>
    void list<T, Alloc>::reverse() {
        if(!empty()) {
            iterator it = begin();
            while(it != end()) {
                iterator tmp = it++;
                transfer(begin(), tmp, it);
            }
        }
    }

    template<typename T, typename Alloc>
    void list<T,Alloc>::sort() {
        
    }
}

#endif //TINY_STL_LF_LIST_H
