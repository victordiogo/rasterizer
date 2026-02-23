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
  union { T x, r; };
  union { T y, g; };

  explicit Vec(T value = T{}) : x{value}, y{value} {}

  Vec(T x, T y) : x{x}, y{y} {}

  auto operator[](unsigned index) -> T& {
    assert(index < 2);
    if (index == 0) return x;
    return y;
  }

  auto operator[](unsigned index) const -> T {
    assert(index < 2);
    if (index == 0) return x;
    return y;
  }
};

template<typename T>
class Vec<T, 3> {
public:
  union { T x, r; };
  union { T y, g; };
  union { T z, b; };

  explicit Vec(T value = T{}) : x{value}, y{value}, z{value} {}

  Vec(T x, T y, T z) : x{x}, y{y}, z{z} {}

  auto operator[](unsigned index) -> T& {
    assert(index < 3);
    if (index == 0) return x;
    if (index == 1) return y;
    return z;
  }

  auto operator[](unsigned index) const -> T {
    assert(index < 3);
    if (index == 0) return x;
    if (index == 1) return y;
    return z;
  }
};

template<typename T>
class Vec<T, 4> {
public:
  union { T x, r; };
  union { T y, g; };
  union { T z, b; };
  union { T w, a; };

  explicit Vec(T value = T{}) : x{value}, y{value}, z{value}, w{value} {}

  Vec(T x, T y, T z, T w) : x{x}, y{y}, z{z}, w{w} {}

  auto operator[](unsigned index) -> T& {
    assert(index < 4);
    if (index == 0) return x;
    if (index == 1) return y;
    if (index == 2) return z;
    return w;
  }

  auto operator[](unsigned index) const -> T {
    assert(index < 4);
    if (index == 0) return x;
    if (index == 1) return y;
    if (index == 2) return z;
    return w;
  }
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
  return Vec<T, dim>{a} / b;
}

template<typename T, int dim>
inline auto operator/=(Vec<T, dim>& a, T b) -> Vec<T, dim>& {
  a = a / b;
  return a;
}

template<typename T, int dim>
inline auto length(const Vec<T, dim>& v) -> T {
  auto sum = 0;
  for (auto i = 0u; i < dim; ++i) {
    sum += v[i] * v[i];
  }
  return std::sqrt(sum);
}

// If the length of v is 0, returns a zero vector
template<typename T, int dim>
inline auto normalize(const Vec<T, dim>& v) -> Vec<T, dim> {
  auto len = length(v);
  if (len == 0) {
    return Vec<T, dim>{0};
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
    a.y * b.z - a.z * b.y,
    a.z * b.x - a.x * b.z,
    a.x * b.y - a.y * b.x
  };
}

template<typename T, int dim>
inline auto dot(const Vec<T, dim>& a, const Vec<T, dim>& b) -> T {
  auto sum = 0;
  for (auto i = 0u; i < dim; ++i) {
    sum += a[i] * b[i];
  }
  return sum;
}


#endif // VECTOR_HPP