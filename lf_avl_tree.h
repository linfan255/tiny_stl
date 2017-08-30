//
// Created by van on 17-8-26.
//

#ifndef TINY_STL_LF_RB_TREE_H
#define TINY_STL_LF_RB_TREE_H

#include "lf_iterator.h"
#include "allocator.h"

namespace lf {
    template <typename T>
    struct _tree_node {
        typedef _tree_node<T>*   node_ptr;
        typedef T                value_type;

        value_type data;
        node_ptr lch, rch, parent;
    };

    inline bool operator==(const _tree_iterator& lhs, const _tree_iterator& rhs) {
        return lhs.ptr == rhs.ptr;
    }

    template <typename T, typename Ref, typename Pointer>
    struct _tree_iterator: public iterator<T, bidirectional_iterator_tag> {
        using typename iterator<T,bidirectional_iterator_tag>::iterator_category;
        using typename iterator<T,bidirectional_iterator_tag>::value_type;
        using typename iterator<T,bidirectional_iterator_tag>::reference;
        using typename iterator<T,bidirectional_iterator_tag>::pointer;
        using typename iterator<T,bidirectional_iterator_tag>::const_reference;
        using typename iterator<T,bidirectional_iterator_tag>::const_pointer;
        using typename iterator<T,bidirectional_iterator_tag>::difference_type;

        typedef _tree_node<T>* node_ptr;
        typedef _tree_iterator<T, T&, T*> iterator;
        typedef _tree_iterator<T, const T&, const T*> const_iterator;
        typedef _tree_iterator<T, Ref, Pointer> self;

        node_ptr ptr;   //真正的指针实现于此，指向节点

        //ctor & dtor
        _tree_iterator() = default;
        _tree_iterator(node_ptr x): ptr(x) {}
        _tree_iterator(const iterator& other): ptr(other.ptr) {}

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
    void _tree_iterator<T,Ref,Pointer>::increment() {
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
    void _tree_iterator<T,Ref,Pointer>::decrement() {
        if(ptr->parent->parent == ptr) {    //如果是root的话，做递减运算后应该指向的是最右边的节点
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


    template <typename Key, typename Value, typename KeyOfValue,
            typename Compare, typename Alloc = allocator<_tree_node<Value>> >
    class avl_tree {
    public:
        typedef _tree_iterator<Value, Value&, Value*> iterator;
        typedef _tree_iterator<Value, const Value&, const Value*> const_iterator;

        typedef _tree_node<Value> node;
        typedef allocator<node> data_allocator;
        typedef node* link_type;

        typedef Key key_type;
        typedef Value value_type;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        typedef value_type *pointer;
        typedef const value_type *const_pointer;
        typedef value_type &reference;
        typedef const value_type &const_reference;

    private:
        link_type header;   //头节点，不含任何数据
        size_type sz;       //树的大小
        Compare key_cmp;    //仿函数对象，用来比较大小

    private:
        //类内部用到的一些辅助函数
        link_type allocate_node(const value_type& x);
        void deallocate_node(link_type node_ptr);
        link_type single_rotate_with_left(link_type x);
        link_type single_rotate_with_right(link_type x);
        link_type double_rotate_with_left(link_type x);
        link_type double_rotate_with_right(link_type x);

        void init_tree();
        void destroy_tree(link_type rt);

    public:
        avl_tree(): header(nullptr), sz(0), key_cmp(Compare()) { init_tree(); }
        ~avl_tree();

        iterator begin() { return header->lch; }
        iterator end() { return header; }
        iterator cbegin() const { return header->lch; }
        iterator cend() const { return header->rch; }

        bool empty() const { return sz == 0; }
        size_type size() const { return sz; }

        value_type front() { return header->lch->data; }
        value_type back() { return header->rch->data; }

        void clear();
        iterator erase(iterator pos);
        iterator erase(iterator start, iterator finish);

        iterator insert_unique(const value_type& x);
        iterator insert_equal(const value_type& x);
    };

    /********************************* implement ***********************************/
    template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    void avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::clear() {
        destroy_tree(header->parent);
    }

    template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::~avl_tree() {
        clear();
        deallocate_node(header);
    }

    template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    void avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::init_tree() {
        header = data_allocator::allocate();
        header->parent = nullptr;
        header->lch = header;
        header->rch = header;
    }

    template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    _tree_node<Value>*
    avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::double_rotate_with_right(link_type x) {
        single_rotate_with_left(x->rch);
        return single_rotate_with_right(x);
    }

    template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    _tree_node<Value>*
    avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::double_rotate_with_left(link_type x) {
        single_rotate_with_right(x->lch);
        return single_rotate_with_left(x);
    }

    template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    _tree_node<Value>*
    avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::single_rotate_with_right(link_type x) {
        link_type rnode = x->rch;
        x->rch = rnode->lch;
        rnode->lch = x;
        return rnode;
    }

    template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    _tree_node<Value>*
    avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::single_rotate_with_left(link_type x) {
        link_type lnode = x->lch;
        x->lch = lnode->rch;
        lnode->rch = x;
        return lnode;
    }

    template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    void avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::deallocate_node(link_type node_ptr) {
        destroy(&node_ptr->data);
        data_allocator::deallocate(node_ptr);
    }

    template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    _tree_node<Value>*
    avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::allocate_node(const value_type &x) {
        link_type p = data_allocator::allocate();
        try {
            construct(&p->data, x);
        } catch (...) {
            data_allocator::deallocate(p);
        }
        return p;
    }
}

#endif //TINY_STL_LF_RB_TREE_H
