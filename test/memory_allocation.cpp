#include <iostream>
#include <array>
#include <memory>
#include <vector>
#include <tuple>

#define START try{
#define END }catch(...){}
void *operator new(std::size_t)
{
    std::puts(__PRETTY_FUNCTION__);
    throw std::bad_alloc();
}
void *operator new(std::size_t, std::nothrow_t) noexcept 
{
    std::puts(__PRETTY_FUNCTION__);
    std::terminate();
}
void *operator new(std::size_t, std::align_val_t)
{
    std::puts(__PRETTY_FUNCTION__);
    throw std::bad_alloc();
}
void *operator new(std::size_t, std::align_val_t, std::nothrow_t) noexcept
{
    std::puts(__PRETTY_FUNCTION__);
    std::terminate();
}

void destroy(int*) {
    std::puts(__PRETTY_FUNCTION__);
}

int main() {

START
    std::vector<int> v;
    std::cout << v.size() << std::endl;
END

START
    std::array<int, 10> v;
    std::cout << v.size() << std::endl;
END

START
    int* p = static_cast<int*>(malloc(sizeof(int)));
    std::unique_ptr<int> v(p);
    std::cout << sizeof(v) << std::endl;
END

START
    int* p = static_cast<int*>(malloc(sizeof(int)));
    std::unique_ptr<int, void(*)(int*)> v(p, destroy);
    std::cout << sizeof(v) << std::endl;
END

START
    int* p = static_cast<int*>(malloc(sizeof(int)));
    auto f = [](int*){
        std::puts(__PRETTY_FUNCTION__);
    };
    std::unique_ptr<int, decltype(f)> v(p, f);
    std::cout << sizeof(v) << std::endl;
END
    // TODO: ...
    // std::tuple<int, double> v;
    // v = 10;
}

