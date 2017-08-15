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
    };  //链表的节点结构

    template<typename T>
    struct _list_iterator: public iterator<bidirectional_iterator_tag,T> {
        typedef _list_iterator<T> list_iterator;
        typedef _list_node<T>* node_ptr;

        node_ptr ptr;   //真正指向_list_node的指针

        //ctor
        _list_iterator(): ptr(nullptr) {}
        explicit _list_iterator(node_ptr _ptr): ptr(_ptr) {}
        _list_iterator(const list_iterator& other): ptr(other.ptr) {}

        const_reference operator*() const { return ptr->data; }
        reference operator*() { return ptr->data; }
        const_pointer operator->() const { return &(operator*()); }
        pointer operator->() { return &(operator*()); }

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






}

#endif //TINY_STL_LF_LIST_H
