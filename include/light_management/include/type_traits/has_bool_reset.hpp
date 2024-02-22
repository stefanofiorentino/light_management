#pragma once

#include <type_traits>

template<typename T, class Enable>
struct has_bool_reset_impl : std::false_type
{
};

template<typename T>
struct has_bool_reset_impl<T, decltype(std::declval<T>().reset())>
  : std::true_type
{
};

template<typename T>
struct has_bool_reset : has_bool_reset_impl<T, bool>
{
};

template<class T>
inline constexpr bool has_bool_reset_v = has_bool_reset<T>::value;
