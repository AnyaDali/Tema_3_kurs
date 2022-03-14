#include <iostream>
#include <atomic>
#include <thread>
#include <functional>
#include <mutex>

template <typename... Args>
void f(const std::function<void(Args&...)> &ptr_func, Args &...args)
{
    for (size_t i = 0; i < 10000000; ++i)
    {
        ptr_func(args...);
    }
}

int main()
{
    {
        size_t i = 0;
        auto ptr_func = [](size_t &i)
        { ++i; };

        std::thread th1(f<size_t>, std::ref(ptr_func), std::ref(i));
        std::thread th2(f<size_t>, std::ref(ptr_func), std::ref(i));
        th1.join();
        th2.join();
        std::cout << i << std::endl;
    }


    {
        std::atomic<size_t> ind{0ULL};

        auto ptr_func = [](std::atomic<size_t> &i)
        { i.fetch_add(1); };

        std::thread th1(f<std::atomic<size_t>>, std::ref(ptr_func), std::ref(ind));
        std::thread th2(f<std::atomic<size_t>>, std::ref(ptr_func), std::ref(ind));
        th1.join();
        th2.join();

        std::cout << ind << std::endl;
    }

    {
        std::size_t ind = 0ULL;
        std::mutex mtx;

        auto ptr_func = [](size_t &i, std::mutex &mtx)
        {
            mtx.lock();
            ++i;
            mtx.unlock();
        };
        std::thread th1(f<size_t, std::mutex>, std::ref(ptr_func), std::ref(ind), std::ref(mtx));
        std::thread th2(f<size_t, std::mutex>, std::ref(ptr_func), std::ref(ind), std::ref(mtx));
        th1.join();
        th2.join();

        std::cout << ind << std::endl;
    }

    return 0;
}