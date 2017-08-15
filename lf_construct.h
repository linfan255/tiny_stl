//
// Created by van on 17-8-10.
//

#ifndef TINY_STL_LF_CONSTRUCT_H
#define TINY_STL_LF_CONSTRUCT_H



namespace lf
{

    template <typename T1, typename T2>
    inline void construct(T1* p, const T2& val) {
        new(p) T1(val);
    }

    template<typename T>
    inline void destroy(T* p) {
        p->~T();
    }

    //还未实现萃取机，暂时区间析构函数都为以下的实现
    template<typename ForwardIter>
    inline void destroy(ForwardIter first, ForwardIter last) {
        for(; first != last; ++first)
            destroy(&*first);
    }


}


#endif //TINY_STL_LF_CONSTRUCT_H
