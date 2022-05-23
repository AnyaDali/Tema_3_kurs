#include <iostream>

template <unsigned V, unsigned T, unsigned S>
struct __pow
{
    static const unsigned value = V * __pow<T, T, S - 1>::value;
};

template <unsigned V, unsigned T>
struct __pow<V, T, 0>
{
    static const unsigned value = 1;
};

template <unsigned V, unsigned S>
const unsigned pow_v = __pow<V, V, S>::value;

template <unsigned S>
const unsigned pow2_v = __pow<2, 2, S>::value;

int main()
{
    std::cout << pow_v<3, 3> << std::endl;
    std::cout << pow2_v<12> << std::endl;
    return 0;
}