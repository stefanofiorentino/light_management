#pragma once

#include <type_traits>
#include "type_traits/has_void_setColor.hpp"
#include "type_traits/has_bool_reset.hpp"
#include "type_traits/has_void_setFullScale.hpp"
#include "type_traits/has_void_draw.hpp"

template <class T>
inline constexpr bool is_light_bulb_v = has_bool_reset_v<T>;

template <class T>
inline constexpr bool is_dimmerable_light_bulb_v = has_bool_reset_v<T> && has_void_setFullScale_v<T>;

template <class T>
inline constexpr bool is_color_light_bulb_v = has_bool_reset_v<T> && has_void_setFullScale_v<T> && has_void_setColor_v<T>;
