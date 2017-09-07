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
        bool isHeader;
        int high;
    };


    template <typename T, typename Ref, typename Pointer>
    struct _tree_iterator {
        typedef bidirectional_iterator_tag iterator_category;
        typedef ptrdiff_t difference_type;
        typedef T value_type;
        typedef value_type& reference;
        typedef const value_type& const_reference;
        typedef value_type* pointer;
        typedef const value_type* const_pointer;

        typedef _tree_node<T>* node_ptr;
        typedef _tree_iterator<T, T&, T*> iterator;
        typedef _tree_iterator<T, const T&, const T*> const_iterator;
        typedef _tree_iterator<T, Ref, Pointer> self;

        node_ptr ptr;   //真正的指针实现于此，指向节点

        //ctor & dtor
        _tree_iterator() = default;
        _tree_iterator(node_ptr x): ptr(x) {}
        _tree_iterator(const iterator& other): ptr(other.ptr) {}

        reference operator*()  { return ptr->data; }
        pointer operator->() { return &(operator*()); }

        const_reference operator*() const { return ptr->data; }
        const_pointer operator->() const { return &(operator*()); }

        bool operator==(const self& rhs) const {
            return ptr == rhs.ptr;
        }

        bool operator!=(const self& rhs) const {
            return ptr != rhs.ptr;
        }

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
        if(ptr->parent->parent == ptr && ptr->isHeader) {
            //如果是header的话，做递减运算后应该指向的是最右边的节点
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

        int height(link_type x) { return x == nullptr ? -1 : x->high; }
        iterator leftmost();

        link_type find_min_node(link_type it) const;
        link_type find_max_node(link_type it) const;

        value_type find_min() const;
        value_type find_max() const;

        void init_tree();
        void destroy_tree(link_type rt);
        link_type _insert_aux(const value_type& x, link_type rt, link_type p, bool unique);
        void _erase_aux(link_type x, link_type rt, link_type p);

    public:
        avl_tree(): header(nullptr), sz(0), key_cmp(Compare()) { init_tree(); }
        ~avl_tree();

        iterator begin() { return leftmost(); }
        iterator end() { return header; }
        iterator cbegin() const { return header->lch; }
        iterator cend() const { return header->rch; }

        bool empty() const { return sz == 0; }
        size_type size() const { return sz; }

        value_type front() { return find_min(); }
        value_type back() { return find_max(); }

        void clear();
        void erase(iterator pos);

        iterator insert_unique(const value_type& x);
        iterator insert_equal(const value_type& x);


        void traverse_tree();   //遍历整个二叉树，只作为测试用
        void _traverse(link_type x);
    };

    /********************************* implement ***********************************/
    template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    void avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::_erase_aux(link_type x, link_type rt, link_type p) {
        if(rt == nullptr)
            return;

        std::cout << "curr node is " << rt->data << " erase " << x->data << std::endl;

        if(x->data == rt->data) {
            if(rt->rch && rt->lch) {
                int random_choose = size() % 2;
                link_type new_root = random_choose == 0 ? find_max_node(rt->lch) :
                                     find_min_node(rt->rch);
                rt->data = new_root->data;

                if(random_choose == 0)  _erase_aux(new_root, rt->lch, rt);
                else                    _erase_aux(new_root, rt->rch, rt);
                return;
            }
            else {
                //如果只有一个子树或者没有子树
                link_type tmp = rt;

                //更新最大值和最小值
                if(rt == header->lch) {
                    if(rt->rch) header->lch = rt->rch;
                    else        header->lch = p;
                }
                if(rt == header->rch) {
                    if(rt->lch) header->rch = rt->lch;
                    else        header->rch = p;
                }

                if(rt->lch != nullptr) {
                    rt = rt->lch;
                    rt->parent = p;
                }
                else if(rt->rch != nullptr){
                    rt = rt->rch;
                    rt->parent = p;
                }
                else {
                    if(p->lch == rt)    p->lch = nullptr;
                    if(p->rch == rt)    p->rch = nullptr;
                }

                deallocate_node(tmp);
                tmp = nullptr;
                --sz;
            }
        }
        else if(key_cmp(KeyOfValue()(x->data), KeyOfValue()(rt->data))) {
            //删除的节点在当前节点左子树中
            _erase_aux(x, rt->lch, rt);
            return;
        }
        else {
            //如果删除的节点在当前节点的右子树当中
            _erase_aux(x, rt->rch, rt);
            return;
        }

        //对失衡的情况进行调整
        if(rt) {
            rt->high = std::max(height(rt->lch), height(rt->rch)) + 1;
            if(height(rt->lch) - height(rt->rch) == 2) {
                //左子树过高
                if(rt->lch->lch)    rt = single_rotate_with_left(rt);
                else                rt = double_rotate_with_left(rt);
            }
            else if(height(rt->rch) - height(rt->lch) == 2) {
                //右子树过高
                if(rt->rch->rch)    rt = single_rotate_with_right(rt);
                else                rt = double_rotate_with_right(rt);
            }
        }
    }

    template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    void avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::erase(iterator pos) {
        std::cout << "erase " << pos.ptr->data << std::endl;
        _erase_aux(pos.ptr, header->parent, header);
    }

    template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    typename avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::link_type
    avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::find_max_node(link_type it) const {
        while(it->rch) {
            it = it->rch;
        }
        return it;
    }

    template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    typename avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::link_type
    avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::find_min_node(link_type it) const {
        while(it->lch)
            it = it->lch;
        return it;
    }

    template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    void avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::_traverse(link_type x) {
        if(x != nullptr) {
            std::cout << "node:" << x->data << std::endl;
            _traverse(x->lch);
            _traverse(x->rch);
        }
    }

    template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    void avl_tree<Key,Value, KeyOfValue,Compare,Alloc>::traverse_tree() {
        _traverse(header->parent);
    }

    template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    typename avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::iterator
    avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::leftmost() {
        link_type p = header->parent;
        while(p->lch != nullptr) {
            p = p->lch;
        }
        return p;
    }

    template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    typename avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::value_type
    avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::find_min() const {
        if(!empty()) {
            return header->lch->data;
        }
    }

    template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    typename avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::value_type
    avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::find_max() const {
        if(!empty()) {
            return header->rch->data;
        }
    }

    template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    typename avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::link_type
    avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::_insert_aux(const value_type &x,
                                                              link_type rt, link_type p, bool unique) {
        if(rt == nullptr) {
            rt = allocate_node(x);
            rt->parent = p;
            rt->isHeader = false;
            sz++;

            if(p == header || x < header->lch->data) header->lch = rt;
            if(p == header || x > header->rch->data) header->rch = rt;

            return rt;
        }

        if(unique && x == rt->data) //如果插入方式是unique的话且与当前节点相同值，则插入失败
            return rt;

        if(key_cmp(KeyOfValue()(x), KeyOfValue()(rt->data))) {
            //新值小于当前节点的值，往左
            //std::cout << KeyOfValue()(x) << " small than " << KeyOfValue()(rt->data) << std::endl;
            rt->lch = _insert_aux(x, rt->lch, rt, unique);
            if(height(rt->lch) - height(rt->rch) == 2)
                if(key_cmp(KeyOfValue()(x), KeyOfValue()(rt->lch->data)))
                    rt = single_rotate_with_left(rt);
                else
                    rt = double_rotate_with_left(rt);
        }
        else {
            rt->rch = _insert_aux(x, rt->rch, rt, unique);
            if(height(rt->rch) - height(rt->lch) == 2)
                if(key_cmp(KeyOfValue()(rt->rch->data), KeyOfValue()(x)))
                    rt = single_rotate_with_right(rt);
                else
                    rt = double_rotate_with_right(rt);
        }

        rt->high = (height(rt->lch) > height(rt->rch) ? height(rt->lch) : height(rt->rch)) + 1;
        return rt;
    }

    template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    typename avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::iterator
    avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::insert_unique(const value_type &x) {
        header->parent = _insert_aux(x, header->parent, header, true);
        header->parent->parent = header;
        return header->parent;
    }

    template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    typename avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::iterator
    avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::insert_equal(const value_type &x) {
        header->parent = _insert_aux(x, header->parent, header, false);
        header->parent->parent = header;
        return header->parent;
    }

    template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    void avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::destroy_tree(link_type rt) {
        if(rt->lch != nullptr)  destroy_tree(rt->lch);
        if(rt->rch != nullptr)  destroy_tree(rt->rch);
        deallocate_node(rt);
    }

    template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    void avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::clear() {
        sz = 0;
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
        header->high = 0;
        header->isHeader = true;
    }

    template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    _tree_node<Value>*
    avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::double_rotate_with_right(link_type x) {
        x->rch = single_rotate_with_left(x->rch);
        x->rch->parent = x;
        return single_rotate_with_right(x);
    }

    template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    _tree_node<Value>*
    avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::double_rotate_with_left(link_type x) {
        x->lch = single_rotate_with_right(x->lch);
        x->lch->parent = x;
        return single_rotate_with_left(x);
    }

    template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    _tree_node<Value>*
    avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::single_rotate_with_right(link_type x) {
        link_type rnode = x->rch;
        x->rch = rnode->lch;
        if(rnode->lch)  rnode->lch->parent = x;

        rnode->lch = x;
        rnode->parent = x->parent;
        x->parent = rnode;

        x->high = (height(x->lch) > height(x->rch) ? height(x->lch) : height(x->rch)) + 1;
        rnode->high = (height(rnode->rch) > height(x) ? height(rnode->rch) : height(x)) + 1;
        return rnode;
    }

    template <typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc>
    _tree_node<Value>*
    avl_tree<Key,Value,KeyOfValue,Compare,Alloc>::single_rotate_with_left(link_type x) {
        link_type lnode = x->lch;
        x->lch = lnode->rch;
        if(lnode->rch)  lnode->rch->parent = x;

        lnode->rch = x;
        lnode->parent = x->parent;
        x->parent = lnode;

        x->high = (height(x->lch) > height(x->rch) ? height(x->lch) : height(x->rch)) + 1;
        lnode->high = (height(lnode->lch) > height(x) ? height(lnode->lch) : height(x)) + 1;
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
        p->high = 0;
        p->lch = p->rch = nullptr;
        return p;
    }
}

#endif //TINY_STL_LF_RB_TREE_H
