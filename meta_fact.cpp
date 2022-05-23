#include <iostream>
#include <semaphore>
template <unsigned long long N>
struct factorial
{
    static const unsigned long long value = N * factorial<N - 1>::value;
};

template <>
struct factorial<0>
{
    static const unsigned long long value = 1;
};

template <unsigned long long N>
const unsigned long long factorial_v = factorial<N>::value;


int main()
{
    std::cout << factorial_v<5> << std::endl;  // constexpr 120
    return 0;
}