cmake_minimum_required(VERSION 3.8)
project(tiny_stl)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp lf_alloc.cpp lf_alloc.h allocator.cpp allocator.h lf_construct.h lf_iterator.h lf_type_traits.h lf_uninitialized.h lf_algorithm.h vector.h lf_list.h lf_deque.h lf_stack.h lf_queue.h lf_avl_tree.h)
add_executable(tiny_stl ${SOURCE_FILES})