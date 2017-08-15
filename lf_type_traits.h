//
// Created by van on 17-8-11.
//

#ifndef TINY_STL_LF_TYPE_TRAITS_H
#define TINY_STL_LF_TYPE_TRAITS_H

namespace lf {
    struct _true_type {};
    struct _false_type {};

    template <typename T>
    struct type_traits{
        typedef _false_type has_trivial_default_ctor;
        typedef _false_type has_trivial_copy_ctor;
        typedef _false_type has_trivial_assignment_operator;
        typedef _false_type has_trivial_dtor;
        typedef _false_type is_pod_type;
    };

    template <>
    struct type_traits<char>{
        typedef _true_type has_trivial_default_ctor;
        typedef _true_type has_trivial_copy_ctor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_dtor;
        typedef _true_type is_pod_type;
    };

    template <>
    struct type_traits<unsigned char>{
        typedef _true_type has_trivial_default_ctor;
        typedef _true_type has_trivial_copy_ctor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_dtor;
        typedef _true_type is_pod_type;
    };


    template <>
    struct type_traits<short>{
        typedef _true_type has_trivial_default_ctor;
        typedef _true_type has_trivial_copy_ctor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_dtor;
        typedef _true_type is_pod_type;
    };

    template <>
    struct type_traits<unsigned short>{
        typedef _true_type has_trivial_default_ctor;
        typedef _true_type has_trivial_copy_ctor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_dtor;
        typedef _true_type is_pod_type;
    };

    template <>
    struct type_traits<int>{
        typedef _true_type has_trivial_default_ctor;
        typedef _true_type has_trivial_copy_ctor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_dtor;
        typedef _true_type is_pod_type;
    };

    template <>
    struct type_traits<unsigned int>{
        typedef _true_type has_trivial_default_ctor;
        typedef _true_type has_trivial_copy_ctor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_dtor;
        typedef _true_type is_pod_type;
    };

    template <>
    struct type_traits<long>{
        typedef _true_type has_trivial_default_ctor;
        typedef _true_type has_trivial_copy_ctor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_dtor;
        typedef _true_type is_pod_type;
    };

    template <>
    struct type_traits<unsigned long>{
        typedef _true_type has_trivial_default_ctor;
        typedef _true_type has_trivial_copy_ctor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_dtor;
        typedef _true_type is_pod_type;
    };

    template <>
    struct type_traits<float>{
        typedef _true_type has_trivial_default_ctor;
        typedef _true_type has_trivial_copy_ctor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_dtor;
        typedef _true_type is_pod_type;
    };

    template <>
    struct type_traits<double>{
        typedef _true_type has_trivial_default_ctor;
        typedef _true_type has_trivial_copy_ctor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_dtor;
        typedef _true_type is_pod_type;
    };

    template <>
    struct type_traits<long double>{
        typedef _true_type has_trivial_default_ctor;
        typedef _true_type has_trivial_copy_ctor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_dtor;
        typedef _true_type is_pod_type;
    };

    template <typename T>
    struct type_traits<T*>{
        typedef _true_type has_trivial_default_ctor;
        typedef _true_type has_trivial_copy_ctor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_dtor;
        typedef _true_type is_pod_type;
    };

    template <typename T>
    struct type_traits<const T*>{
        typedef _true_type has_trivial_default_ctor;
        typedef _true_type has_trivial_copy_ctor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_dtor;
        typedef _true_type is_pod_type;
    };
}

#endif //TINY_STL_LF_TYPE_TRAITS_H
