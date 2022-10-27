#pragma once

#include <scoped_allocator>

#include "arena_allocator.h"

template <typename T>
using SA = std::scoped_allocator_adaptor<ArenaAllocator<T>>;
