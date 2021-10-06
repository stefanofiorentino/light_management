#pragma once

#include <iostream>
#include <type_traits>

template <typename T, class Enable>
struct has_int_value_impl : std::false_type
{
};

template <typename T>
struct has_int_value_impl<T, decltype(std::declval<T>().value())> : std::true_type
{
};

template <typename T>
struct has_int_value : has_int_value_impl<T, int>
{
};

template <class T>
inline constexpr bool has_int_value_v = has_int_value<T>::value;
