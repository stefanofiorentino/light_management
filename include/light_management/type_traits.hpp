#pragma once

#include "type_traits/has_bool_reset.hpp"
#include "type_traits/has_return_code_do_switch.hpp"
#include "type_traits/has_void_draw.hpp"
#include "type_traits/has_void_setColor.hpp"
#include "type_traits/has_void_setFullScale.hpp"
#include <type_traits>

struct CoverageFailure
{
  virtual bool inCall() const = 0;
  bool status() const {
    return false;
  }
  virtual ~CoverageFailure() = default;
};

template<class T>
inline constexpr bool is_light_bulb_v = has_bool_reset_v<T>;

template<class T>
inline constexpr bool is_dimmable_light_bulb_v =
  has_bool_reset_v<T>&& has_void_setFullScale_v<T>;

template<class T>
inline constexpr bool is_color_light_bulb_v =
  has_bool_reset_v<T>&& has_void_setFullScale_v<T>&& has_void_setColor_v<T>;
