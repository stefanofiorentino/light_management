#include <gmock/gmock.h>

#include <cstddef>
#include <iostream>
#include <map>
#include <scoped_allocator>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

namespace experimental {
namespace constants {
static const std::string &VERY_LONG_STRING = "Very looooooooooooooong string";
}
} // namespace experimental

class Arena {
  unsigned char *const data;
  std::size_t const size;
  std::size_t offset;

public:
  explicit Arena(std::size_t s)
      : data(static_cast<unsigned char *>(::operator new(s))), size(s),
        offset(0) {
    std::cout << "arena[" << this << "] of size " << size << " created.\n";
  }

  Arena(Arena const &) = delete;
  Arena &operator=(Arena const &) = delete;

  ~Arena() {
    std::cout << "arena[" << this
              << "] destroyed; final fill level was: " << offset << "\n";
    ::operator delete(data);
  }

  void *allocate(std::size_t n, std::size_t a) {
    offset = (offset + a - 1) / a * a;

    std::cout << "arena[" << this << "] allocating " << n << " bytes at offset "
              << offset << ".\n";

    if (offset + n > size) {
      throw std::bad_alloc();
    }

    void *result = data + offset;
    offset += n;
    return result;
  }

  void deallocate(void *, std::size_t n) {
    std::cout << "arena[" << this << "] may deallocate " << n << " bytes.\n";
  }
};

template <typename T> struct ArenaAllocator {
  template <typename U> friend struct ArenaAllocator;

  using value_type = T;
  using pointer = T *;

  using propagate_on_container_copy_assignment = std::true_type;
  using propagate_on_container_move_assignment = std::true_type;
  using propagate_on_container_swap = std::true_type;

  explicit ArenaAllocator(Arena *a) : arena(a) {}

  template <typename U>
  ArenaAllocator(ArenaAllocator<U> const &rhs) : arena(rhs.arena) {}

  pointer allocate(std::size_t n) {
    return static_cast<pointer>(arena->allocate(n * sizeof(T), alignof(T)));
  }

  void deallocate(pointer p, std::size_t n) {
    arena->deallocate(p, n * sizeof(T));
  }

  template <typename U> bool operator==(ArenaAllocator<U> const &rhs) const {
    return arena == rhs.arena;
  }

  template <typename U> bool operator!=(ArenaAllocator<U> const &rhs) const {
    return !(*this == rhs);
  }

private:
  Arena *arena;
};

template <typename T>
using SA = std::scoped_allocator_adaptor<ArenaAllocator<T>>;
using astring = std::basic_string<char, std::char_traits<char>, SA<char>>;

TEST(visitor_guide_allocators, visitor_guide_allocators) {
  Arena arena(1024);
  ArenaAllocator<void> a(&arena);
  astring s2(experimental::constants::VERY_LONG_STRING, a);
  ASSERT_EQ(experimental::constants::VERY_LONG_STRING, s2.c_str());
}