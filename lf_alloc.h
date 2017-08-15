//
// Created by van on 17-8-10.
//

#ifndef TINY_STL_ALLOC_H
#define TINY_STL_ALLOC_H


#include <cstdlib>
#include <iostream>

namespace lf {
    /*用内存池的方式实现alloc，被simple_alloc调用*/

    class lf_alloc {
        enum ENAlign {_ALIGN = 8};
        enum ENMaxByte {_MAXBYTE = 128};
        enum ENFreeListsNum {_FREELIST_NUM = 16};

    private:
        union list_node {
            union list_node* next;
            char data[1];
        };
        static list_node* free_list[ENFreeListsNum::_FREELIST_NUM];	//链表集合

        static char* free_start;	//内存池
        static char* free_end;
        static size_t heap_size;

        //一些辅助性的函数
        static size_t ROUND_UP(size_t n) {
            return ((n + ENAlign::_ALIGN - 1) & ~(ENAlign::_ALIGN - 1));
        }
        static size_t FREELIST_ID(size_t n) {
            return ((n + ENAlign::_ALIGN - 1) / ENAlign::_ALIGN - 1);
        }

        static void* refill(size_t n);					//当区块链表中无法满足用户使用的时候，重新填补
        static char* chunk_alloc(size_t sz, int& n);	//从内存池中分配空间给free_list

    public:
        static void* allocate	(size_t sz);
        static void deallocate	(void* p, size_t sz);
        static void* reallocate (void* p, size_t old_sz, size_t new_sz);

    };

}


#endif //TINY_STL_ALLOC_H
