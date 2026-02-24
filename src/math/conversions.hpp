#ifndef MATH_CONVERSIONS_HPP
#define MATH_CONVERSIONS_HPP

template<typename T>
inline auto radians(T degrees) -> T {
  return degrees * (T)(3.14159265358979323846 / 180.0);
}

template<typename T>
inline auto degrees(T radians) -> T {
  return radians * (T)(180.0 / 3.14159265358979323846);
}

#endif // MATH_CONVERSIONS_HPP