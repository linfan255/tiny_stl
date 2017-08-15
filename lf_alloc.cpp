//
// Created by van on 17-8-10.
//

#include "lf_alloc.h"

namespace lf {


    lf_alloc::list_node* lf_alloc::free_list[] = {
            nullptr, nullptr, nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr
    };


    char* lf_alloc::free_start = nullptr;
    char* lf_alloc::free_end = nullptr;
    size_t lf_alloc::heap_size = 0;


    char* lf_alloc::chunk_alloc(size_t sz, int& n) {
        size_t left_byte = free_end - free_start;	//内存池中剩下的字节数
        size_t alloc_byte = sz * n;					//要为freelist分配的字节数
        char* ret = nullptr;

        if(left_byte >= alloc_byte) {	//如果剩余的内存池容量比所申请的容量大
            char* ret = free_start;
            free_start += alloc_byte;
            return ret;
        } else if(left_byte >= sz) {	//如果能够分配一块或者以上的内存块。
            n = left_byte / sz;			//需要重新调整分配的块数
            alloc_byte = n * sz;
            ret = free_start;
            free_start += alloc_byte;
            return ret;
        } else {	//如果无法供应一个块
            size_t malloc_size = 2 * alloc_byte;
            if(left_byte > 0) {
                //在重新分配前处理剩下的零头
                list_node ** my_free_list = free_list + FREELIST_ID(left_byte);
                ((list_node*)free_start)->next = *my_free_list;
                *my_free_list = (list_node*)free_start;
            }

            free_start = (char*)malloc(malloc_size);
            if(0 == free_start) {
                //如果内存不足
                list_node ** my_free_list, *p;
                for(int i = sz; i <= ENMaxByte::_MAXBYTE; i += ENAlign::_ALIGN) {
                    my_free_list = free_list + FREELIST_ID(i);
                    if(0 != *my_free_list) {
                        p = *my_free_list;
                        *my_free_list = p->next;
                        free_start = (char*)p;
                        free_end = free_start + i;

                        return chunk_alloc(sz, n);
                    }
                }
                free_end = free_start = 0;
                return free_end;
            } else {
                free_end = free_start + malloc_size;
                heap_size += malloc_size;
                return chunk_alloc(sz, n);
            }
        }
    }

    //当在free_list中找不到可用的区块时，调用此函数重新给free_list分配空间
    //该函数会调用chunk_alloc，得到的是一整块未划分区块的内存，大小是sz * n
    //n默认是20
    void* lf_alloc::refill(size_t sz) {
        int n = 20;
        char* ret = chunk_alloc(sz, n);

        if(1 == n)	return (void*)ret;

        list_node ** my_free_list = free_list + FREELIST_ID(sz);

        //如果通过chunk_alloc得到的内存大于一个块，那么就需要分块
        list_node* curr_node = (list_node*)(ret + sz);
        *my_free_list = curr_node;
        list_node* next_node;
        for(int i = 1; i < n; i++) {
            if(i == n - 1) {
                curr_node->next = 0;
            } else {
                next_node = (list_node*)((char*)curr_node + sz);
                curr_node->next = next_node;
                curr_node = curr_node->next;
            }
        }
        return (void*)ret;
    }

    //从free_list中取出sz大小的内存块，sz未调整为8的倍数
    void* lf_alloc::allocate(size_t sz) {
        if(sz > ENMaxByte::_MAXBYTE)
            return malloc(sz);

        list_node ** my_free_list = free_list + FREELIST_ID(sz);
        list_node* ret = *my_free_list;
        if(0 != ret) {
            //有空闲区块
            *my_free_list = (*my_free_list)->next;
            return ret;
        } else {
            //如果没有空闲区块，通过refill向内存池申请
            ret = (list_node*)refill(ROUND_UP(sz));
            return ret;
        }
    }

    //将内存中位置p，大小为sz个字节的内存返还给free_list
    void lf_alloc::deallocate(void* p, size_t sz) {
        if(sz > ENMaxByte::_MAXBYTE) {
            free(p);
            return;
        }

        list_node* pointer = (list_node*)p;
        list_node ** my_free_list = free_list + FREELIST_ID(sz);
        pointer->next = *my_free_list;
        *my_free_list = pointer;
    }

    void* lf_alloc::reallocate(void* p, size_t old_sz, size_t new_sz) {
        deallocate(p, old_sz);
        return allocate(new_sz);
    }
}


