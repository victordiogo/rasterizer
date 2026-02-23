#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <array>
#include "math/vector.hpp"

// row-major order
template<typename T, int dim>
using Mat = std::array<std::array<T, dim>, dim>;

using Mat3f = Mat<float, 3>;
using Mat4f = Mat<float, 4>;

template<typename T, int dim>
inline auto identity() -> Mat<T, dim> {
  auto result = Mat<T, dim>{};
  for (auto i = 0u; i < dim; ++i) {
    result[i][i] = (T)1;
  }
  return result;
}

template<typename T, int dim>
inline auto operator*(const Mat<T, dim>& a, const Mat<T, dim>& b) -> Mat<T, dim> {
  auto result = Mat<T, dim>{};
  for (auto i = 0u; i < dim; ++i) {
    for (auto j = 0u; j < dim; ++j) {
      for (auto k = 0u; k < dim; ++k) {
        result[i][j] += a[i][k] * b[k][j];
      }
    }
  }
  return result;
}

template<typename T, int dim>
inline auto operator*=(Mat<T, dim>& a, const Mat<T, dim>& b) -> Mat<T, dim>& {
  a = a * b;
  return a;
}

template<typename T, int dim>
inline auto operator*(const Vec<T, dim>& v, const Mat<T, dim>& m) -> Vec<T, dim> {
  auto result = Vec<T, dim>{};
  for (auto i = 0u; i < dim; ++i) {
    for (auto j = 0u; j < dim; ++j) {
      result[i] += v[j] * m[j][i];
    }
  }
  return result;
}


#endif // MATRIX_HPP