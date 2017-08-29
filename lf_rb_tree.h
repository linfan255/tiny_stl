//
// Created by van on 17-8-26.
//

#ifndef TINY_STL_LF_RB_TREE_H
#define TINY_STL_LF_RB_TREE_H

#include "lf_iterator.h"

namespace lf {
    typedef bool _rb_tree_color;
    const _rb_tree_color _red_node = true;
    const _rb_tree_color _black_node = false;

    template <typename T>
    struct _rb_tree_node {
        typedef _rb_tree_node<T>*   node_ptr;
        typedef T                   value_type;
        typedef _rb_tree_color      color_type;

        value_type data;
        node_ptr lch, rch, parent;
        color_type color;
    };

    inline bool operator==(const _rb_tree_iterator& lhs, const _rb_tree_iterator& rhs) {
        return lhs.ptr == rhs.ptr;
    }

    template <typename T, typename Ref, typename Pointer>
    struct _rb_tree_iterator: public iterator<T, bidirectional_iterator_tag> {
        using typename iterator<T,bidirectional_iterator_tag>::iterator_category;
        using typename iterator<T,bidirectional_iterator_tag>::value_type;
        using typename iterator<T,bidirectional_iterator_tag>::reference;
        using typename iterator<T,bidirectional_iterator_tag>::pointer;
        using typename iterator<T,bidirectional_iterator_tag>::const_reference;
        using typename iterator<T,bidirectional_iterator_tag>::const_pointer;
        using typename iterator<T,bidirectional_iterator_tag>::difference_type;

        typedef _rb_tree_node<T>* node_ptr;
        typedef _rb_tree_iterator<T, T&, T*> iterator;
        typedef _rb_tree_iterator<T, const T&, const T*> const_iterator;
        typedef _rb_tree_iterator<T, Ref, Pointer> self;

        node_ptr ptr;   //真正的指针实现于此，指向节点

        //ctor & dtor
        _rb_tree_iterator() = default;
        explicit _rb_tree_iterator(node_ptr x): ptr(x) {}
        _rb_tree_iterator(const iterator& other): ptr(other.ptr) {}

        reference operator*() { return ptr->data; }
        pointer operator->() { return &(operator*()); }

        const_reference operator*() const { return ptr->data; }
        const_pointer operator->() const { return &(operator*()); }

        //递增 递减
        self& operator++() {
            increment();
            return *this;
        }

        self operator++(int) {
            self ret = *this;
            increment();
            return ret;
        }

        self& operator--() {
            decrement();
            return *this;
        }

        self operator--(int) {
            self ret = *this;
            decrement();
            return ret;
        }

        void increment();
        void decrement();
    };

    template <typename T, typename Ref, typename Pointer>
    void _rb_tree_iterator<T,Ref,Pointer>::increment() {
        if(ptr->rch != nullptr) {
            node_ptr tmp = ptr->rch;
            while(tmp->lch != nullptr)
                tmp = tmp->lch;
            ptr = tmp;
        }
        else {
            node_ptr p = ptr->parent;
            while(p->rch == ptr) {
                ptr = p;
                p = ptr->parent;
            }
            if(ptr->rch != p)   //一路往上找寻不是右儿子的节点，没有到达header的时候
                ptr = p;
            //假如到达了header，说明是在对没有右儿子的根节点进行递增
        }
    }

    template <typename T, typename Ref, typename Pointer>
    void _rb_tree_iterator<T,Ref,Pointer>::decrement() {
        if(ptr->color == _red_node && ptr->parent->parent == ptr) {
            ptr = ptr->rch;
        }
        else if(ptr->lch != nullptr) {
            node_ptr tmp = ptr->lch;
            while(tmp->rch != nullptr)
                tmp = tmp->rch;
            ptr = tmp;
        }
        else {
            node_ptr p = ptr->parent;
            while(ptr == p->lch) {
                ptr = p;
                p = p->parent;
            }
            ptr = p;
        }
    }
}

#endif //TINY_STL_LF_RB_TREE_H
