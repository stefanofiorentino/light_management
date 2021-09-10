#pragma once

#include <type_traits>

template <typename T, class Enable>
struct has_bool_reset_impl : std::false_type
{
};

template <typename T>
struct has_bool_reset_impl<T, decltype(std::declval<T>().reset())> : std::true_type
{
};

template <typename T>
struct has_bool_reset : has_bool_reset_impl<T, bool>
{
};

template <class T>
inline constexpr bool has_bool_reset_v = has_bool_reset<T>::value;

template <typename T, class Enable>
struct has_void_setFullScale_impl : std::false_type
{
};

template <typename T>
struct has_void_setFullScale_impl<T, decltype(std::declval<T>().setFullScale(0.f))> : std::true_type
{
};

template <typename T>
struct has_void_setFullScale : has_void_setFullScale_impl<T, void>
{
};

template <class T>
inline constexpr bool has_void_setFullScale_v = has_void_setFullScale<T>::value;

template <class T>
inline constexpr bool is_color_light_bulb_v = has_bool_reset_v<T> && has_void_setFullScale_v<T>;
