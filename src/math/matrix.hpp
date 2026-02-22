#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <array>

template<typename T, int size>
using Mat = std::array<std::array<T, size>, size>;

using Mat3f = Mat<float, 3>;
using Mat4f = Mat<float, 4>;

template<typename T, int size>
inline auto identity() -> Mat<T, size> {
  auto result = Mat<T, size>{};
  for (auto i = 0u; i < size; ++i) {
    result[i][i] = (T)1;
  }
  return result;
}

template<typename T, int size>
inline auto operator*(const Mat<T, size>& a, const Mat<T, size>& b) -> Mat<T, size> {
  auto result = Mat<T, size>{};
  for (auto i = 0u; i < size; ++i) {
    for (auto j = 0u; j < size; ++j) {
      for (auto k = 0u; k < size; ++k) {
        result[i][j] += a[i][k] * b[k][j];
      }
    }
  }
  return result;
}

template<typename T, int size>
inline auto operator*=(Mat<T, size>& a, const Mat<T, size>& b) -> Mat<T, size>& {
  a = a * b;
  return a;
}


#endif // MATRIX_HPP