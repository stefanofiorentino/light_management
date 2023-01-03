#pragma once

template<typename T, class Enable>
struct has_void_setFullScale_impl : std::false_type
{};

template<typename T>
struct has_void_setFullScale_impl<T, decltype(std::declval<T>().setFullScale())>
  : std::true_type
{};

template<typename T>
struct has_void_setFullScale : has_void_setFullScale_impl<T, void>
{};

template<class T>
inline constexpr bool has_void_setFullScale_v = has_void_setFullScale<T>::value;
