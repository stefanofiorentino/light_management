#pragma once

#include <type_traits>

template<typename T, class Enable>
struct has_void_setColor_impl : std::false_type
{};

template<typename T>
struct has_void_setColor_impl<T, decltype(std::declval<T>().setColor())>
  : std::true_type
{};

template<typename T>
struct has_void_setColor : has_void_setColor_impl<T, void>
{};

template<class T>
inline constexpr bool has_void_setColor_v = has_void_setColor<T>::value;