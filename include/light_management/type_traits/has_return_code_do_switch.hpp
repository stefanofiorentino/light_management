#pragma once

#include <type_traits>

template<typename T, class Enable>
struct has_void_do_switch_impl : std::false_type
{};

template<typename T>
struct has_void_do_switch_impl<T, decltype(std::declval<T>().do_switch(false))>
  : std::true_type
{};

template<typename T>
struct has_void_do_switch : has_void_do_switch_impl<T, void>
{};

template<class T>
inline constexpr bool has_void_do_switch_v = has_void_do_switch<T>::value;
