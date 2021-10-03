#pragma once

#include <iostream>
#include <type_traits>

template <typename T, class Enable>
struct has_void_switchOff_impl : std::false_type
{
};

template <typename T>
struct has_void_switchOff_impl<T, decltype(std::declval<T>().switchOff(std::cout, size_t()))> : std::true_type
{
};

template <typename T>
struct has_void_switchOff : has_void_switchOff_impl<T, void>
{
};

template <class T>
inline constexpr bool has_void_switchOff_v = has_void_switchOff<T>::value;
