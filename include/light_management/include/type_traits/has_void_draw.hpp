#pragma once

#include <iostream>
#include <type_traits>

template<typename T, class Enable>
struct has_void_draw_impl : std::false_type
{
};

template<typename T>
struct has_void_draw_impl<T, decltype(std::declval<T>().draw(std::cout))>
  : std::true_type
{
};

template<typename T>
struct has_void_draw : has_void_draw_impl<T, void>
{
};

template<class T>
inline constexpr bool has_void_draw_v = has_void_draw<T>::value;
