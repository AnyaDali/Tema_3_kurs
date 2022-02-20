#include <iostream>

template <typename T, typename V>
struct is_same
{
    static const bool value = false;
};

template <typename T>
struct is_same<T, T>
{
    static const bool value = true;
};

template <typename T, typename U>
const bool is_same_v = is_same<T, U>::value;

template <typename T>
struct remove_const
{
    using type = T;
};

template <typename T>
struct remove_const<const T>
{
    using type = T;
};

template <typename T>
using remove_const_t = typename remove_const<T>::type;

template <typename T>
struct remove_reference
{
    using type = T;
};

template <typename T>
struct remove_reference<T &>
{
    using type = T;
};

template <typename T>
struct remove_pointer
{
    using type = T;
};

template <typename T>
struct remove_pointer<T *>
{
    using type = T;
};

template <typename T>
using remove_pointer_t = typename remove_pointer<T>::type;

template <typename First, typename Second, typename... Args>
struct __is_homogeneous
{
    static const bool value = is_same_v<First, Second> && __is_homogeneous<Second, Args...>::value;
};

template <typename First, typename Second>
struct __is_homogeneous<First, Second>
{
    static const bool value = is_same_v<First, Second>;
};

template <typename... Args>
struct is_homogeneous
{
    static const bool value = __is_homogeneous<Args...>::value;
};

template <typename T>
struct is_homogeneous<T>
{
    static const bool value = true;
};

template <>
struct is_homogeneous<>
{
    static const bool value = false;
};

template <typename... Args>
const bool is_homogeneous_v = is_homogeneous<Args...>::value;

// exemple using remove_const type.
template <typename T>
void f(T t)
{
    remove_const_t<T> x = t;
    x += x;
    std::cout << x << std::endl;
}


void print_arg() {}

template <typename Head, typename... Tail>
void print_arg(const Head &head, const Tail &...tail)
{
    std::cout << head << std::endl;
    print_arg(tail...);
}

int main()
{
    f<const int>(20);
    std::cout << is_same_v<int, double> << std::endl;
    print_arg(1, 2, 3, 4, "abs", 1.9f);
    std::cout << is_homogeneous_v<int, int, int, int, int64_t, int> << std::endl;
    return 0;
}