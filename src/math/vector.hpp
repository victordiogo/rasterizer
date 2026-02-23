#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cmath>
#include <array>
#include <cassert>

template<typename T, int dim>
class Vec {
public:
  explicit Vec(T value = T{}) {
    for (auto i = 0u; i < dim; ++i) {
      m_values[i] = value;
    }
  }

  auto operator[](unsigned index) -> T& {
    assert(index < dim);
    return m_values[index];
  }

  auto operator[](unsigned index) const -> T {
    assert(index < dim);
    return m_values[index];
  }

private:
  std::array<T, dim> m_values;
};

template<typename T>
class Vec<T, 2> {
public:
  explicit Vec(T value = T{}) : m_values{value, value} {}

  Vec(T x, T y) : m_values{x, y} {}

  auto x() -> T& { return m_values[0]; }
  auto y() -> T& { return m_values[1]; }
  auto x() const -> T { return m_values[0]; }
  auto y() const -> T { return m_values[1]; }

  auto operator[](unsigned index) -> T& {
    assert(index < 2);
    return m_values[index];
  }

  auto operator[](unsigned index) const -> T {
    assert(index < 2);
    return m_values[index];
  }

private:
  std::array<T, 2> m_values;
};

template<typename T>
class Vec<T, 3> {
public:
  explicit Vec(T value = T{}) : m_values{value, value, value} {}

  Vec(T x, T y, T z) : m_values{x, y, z} {}

  auto x() -> T& { return m_values[0]; }
  auto y() -> T& { return m_values[1]; }
  auto z() -> T& { return m_values[2]; }
  auto x() const -> T { return m_values[0]; }
  auto y() const -> T { return m_values[1]; }
  auto z() const -> T { return m_values[2]; }

  auto operator[](unsigned index) -> T& {
    assert(index < 3);
    return m_values[index];
  }

  auto operator[](unsigned index) const -> T {
    assert(index < 3);
    return m_values[index];
  }

private:
  std::array<T, 3> m_values;
};

template<typename T>
class Vec<T, 4> {
public:
  explicit Vec(T value = T{}) : m_values{value, value, value, value} {}

  Vec(T x, T y, T z, T w) : m_values{x, y, z, w} {}

  auto x() -> T& { return m_values[0]; }
  auto y() -> T& { return m_values[1]; }
  auto z() -> T& { return m_values[2]; }
  auto w() -> T& { return m_values[3]; }
  auto x() const -> T { return m_values[0]; }
  auto y() const -> T { return m_values[1]; }
  auto z() const -> T { return m_values[2]; }
  auto w() const -> T { return m_values[3]; }

  auto operator[](unsigned index) -> T& {
    assert(index < 4);
    return m_values[index];
  }

  auto operator[](unsigned index) const -> T {
    assert(index < 4);
    return m_values[index];
  }

private:
  std::array<T, 4> m_values;
};

using Vec2f = Vec<float, 2>;
using Vec3f = Vec<float, 3>;
using Vec4f = Vec<float, 4>;

template<typename T, int dim>
inline auto operator+(const Vec<T, dim>& a, const Vec<T, dim>& b) -> Vec<T, dim> {
  Vec<T, dim> result;
  for (auto i = 0u; i < dim; ++i) {
    result[i] = a[i] + b[i];
  }
  return result;
}

template<typename T, int dim>
inline auto operator+=(Vec<T, dim>& a, const Vec<T, dim>& b) -> Vec<T, dim>& {
  a = a + b;
  return a;
}

template<typename T, int dim>
inline auto operator-(const Vec<T, dim>& a, const Vec<T, dim>& b) -> Vec<T, dim> {
  Vec<T, dim> result;
  for (auto i = 0u; i < dim; ++i) {
    result[i] = a[i] - b[i];
  }
  return result;
}

template<typename T, int dim>
inline auto operator-=(Vec<T, dim>& a, const Vec<T, dim>& b) -> Vec<T, dim>& {
  a = a - b;
  return a;
}

template<typename T, int dim>
inline auto operator*(const Vec<T, dim>& a, const Vec<T, dim>& b) -> Vec<T, dim> {
  Vec<T, dim> result;
  for (auto i = 0u; i < dim; ++i) {
    result[i] = a[i] * b[i];
  }
  return result;
}

template<typename T, int dim>
inline auto operator*=(Vec<T, dim>& a, const Vec<T, dim>& b) -> Vec<T, dim>& {
  a = a * b;
  return a;
}

template<typename T, int dim>
inline auto operator*(const Vec<T, dim>& a, T b) -> Vec<T, dim> {
  Vec<T, dim> result;
  for (auto i = 0u; i < dim; ++i) {
    result[i] = a[i] * b;
  }
  return result;
}

template<typename T, int dim>
inline auto operator*(T a, const Vec<T, dim>& b) -> Vec<T, dim> {
  return b * a;
}

template<typename T, int dim>
inline auto operator*=(Vec<T, dim>& a, T b) -> Vec<T, dim>& {
  a = a * b;
  return a;
}

template<typename T, int dim>
inline auto operator/(const Vec<T, dim>& a, const Vec<T, dim>& b) -> Vec<T, dim> {
  Vec<T, dim> result;
  for (auto i = 0u; i < dim; ++i) {
    result[i] = a[i] / b[i];
  }
  return result;
}

template<typename T, int dim>
inline auto operator/=(Vec<T, dim>& a, const Vec<T, dim>& b) -> Vec<T, dim>& {
  a = a / b;
  return a;
}

template<typename T, int dim>
inline auto operator/(const Vec<T, dim>& a, T b) -> Vec<T, dim> {
  Vec<T, dim> result;
  for (auto i = 0u; i < dim; ++i) {
    result[i] = a[i] / b;
  }
  return result;
}

template<typename T, int dim>
inline auto operator/(T a, const Vec<T, dim>& b) -> Vec<T, dim> {
  return b / a;
}

template<typename T, int dim>
inline auto operator/=(Vec<T, dim>& a, T b) -> Vec<T, dim>& {
  a = a / b;
  return a;
}

template<typename T, int dim>
inline auto length(const Vec<T, dim>& v) -> T {
  auto sum = (T)0;
  for (auto i = 0u; i < dim; ++i) {
    sum += v[i] * v[i];
  }
  return std::sqrt(sum);
}

// If the length of v is 0, returns a zero initialized vector
template<typename T, int dim>
inline auto normalize(const Vec<T, dim>& v) -> Vec<T, dim> {
  auto len = length(v);
  if (len == (T)0) {
    return Vec<T, dim>{};
  }
  Vec<T, dim> result;
  for (auto i = 0u; i < dim; ++i) {
    result[i] = v[i] / len;
  }
  return result;
}

template<typename T>
inline auto cross(const Vec<T, 3>& a, const Vec<T, 3>& b) -> Vec<T, 3> {
  return Vec<T, 3>{
    a.y() * b.z() - a.z() * b.y(),
    a.z() * b.x() - a.x() * b.z(),
    a.x() * b.y() - a.y() * b.x()
  };
}

#endif // VECTOR_HPP