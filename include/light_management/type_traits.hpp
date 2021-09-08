#pragma once

#include <type_traits>

template <typename T, class Enable>
struct is_color_light_bulb_impl : std::false_type
{
};

template <typename T>
struct is_color_light_bulb_impl<T, decltype(std::declval<T>().reset())> : std::true_type
{
};

template <typename T>
struct is_color_light_bulb : is_color_light_bulb_impl<T, bool>
{
};

template <class T>
inline constexpr bool is_color_light_bulb_v = is_color_light_bulb<T>::value;
