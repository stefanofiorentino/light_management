#include <iostream>
#include <array>
#include <memory>
#include <vector>
#include <tuple>

void *operator new(std::size_t) = delete;
// {
//     std::puts(__PRETTY_FUNCTION__);
//     throw std::bad_alloc();
// }
void *operator new(std::size_t, std::nothrow_t) noexcept = delete;
// {
//     std::puts(__PRETTY_FUNCTION__);
//     std::terminate();
// }
void *operator new(std::size_t, std::align_val_t) = delete;
// {
//     std::puts(__PRETTY_FUNCTION__);
//     throw std::bad_alloc();
// }
void *operator new(std::size_t, std::align_val_t, std::nothrow_t) noexcept = delete;
// {
//     std::puts(__PRETTY_FUNCTION__);
//     std::terminate();
// }

// cppreference> std::vector is a sequence container that encapsulates dynamic size arrays
// RAII 
// TEST(memory_allocation_vector, memory_allocation_vector)
// {
//     std::vector<int> v{10,0};
//     std::cout << v.size() << std::endl;
// }

// TEST(memory_allocation_array, memory_allocation_array)
// {
//     std::array<int, 10> v;
//     std::cout << v.size() << std::endl;
// }

// TEST(memory_allocation_shared_ptr, memory_allocation_shared_ptr)
// {
//     int* p = static_cast<int*>(malloc(sizeof(int)));
//     std::shared_ptr<int> v(p);
// }

void destroy(int*) {
    std::puts(__PRETTY_FUNCTION__);
}

int main() {
    // std::vector<int> v;
    // std::cout << v.size() << std::endl;

    // std::array<int, 10> v;
    // std::cout << v.size() << std::endl;
    
    // int* p = static_cast<int*>(malloc(sizeof(int)));
    // std::shared_ptr<int> v(p);

    // int* p = static_cast<int*>(malloc(sizeof(int)));
    // std::unique_ptr<int> v1(p);
    // std::cout << sizeof(v1) << std::endl;

    // std::unique_ptr<int, void(*)(int*)> v2(p, destroy);
    // std::cout << sizeof(v2) << std::endl;

    // int a = 0;
    // auto f = [&a](int*){
    //     std::puts(__PRETTY_FUNCTION__);
    // };
    // std::unique_ptr<int, decltype(f)> v(p, f);
    // std::cout << sizeof(v) << std::endl;

    // TODO: ...
    // std::tuple<int, double> v;
    // v = 10;
}

