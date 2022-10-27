#pragma once

#include <type_traits>

#include "arena.h"

template<typename T>
struct ArenaAllocator
{
  template<typename U>
  friend struct ArenaAllocator;

  using value_type = T;
  using pointer = T*;

  using propagate_on_container_copy_assignment = std::true_type;
  using propagate_on_container_move_assignment = std::true_type;
  using propagate_on_container_swap = std::true_type;

  explicit ArenaAllocator(Arena* a) noexcept
    : arena(a)
  {}

  template<typename U>
  ArenaAllocator(ArenaAllocator<U> const& rhs) noexcept
    : arena(rhs.arena)
  {}

  pointer allocate(std::size_t n)
  {
    return static_cast<pointer>(arena->allocate(n * sizeof(T), alignof(T)));
  }

  void deallocate(pointer p, std::size_t n) noexcept
  {
    arena->deallocate(p, n * sizeof(T));
  }

  template<typename U>
  bool operator==(ArenaAllocator<U> const& rhs) const noexcept
  {
    return arena == rhs.arena;
  }

  template<typename U>
  bool operator!=(ArenaAllocator<U> const& rhs) const noexcept
  {
    return arena != rhs.arena;
  }

private:
  Arena* arena;
};
