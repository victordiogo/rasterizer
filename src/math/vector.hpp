#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cmath>
#include <array>
#include <cassert>

template<typename T, int dim>
class Vec {
public:
  T& operator[](int index) {
    assert(index >= 0 && index < dim);
    return values[(unsigned)index];
  }

  T operator[](int index) const {
    assert(index >= 0 && index < dim);
    return values[(unsigned)index];
  }

protected:
  std::array<T, dim> values;
};

template<typename T>
class Vec2 : public Vec<T, 2> {
public:
  T& x() { return this->values[0]; }
  T& y() { return this->values[1]; }
  T x() const { return this->values[0]; }
  T y() const { return this->values[1]; }
};

template<typename T>
class Vec3 : public Vec<T, 3> {
public:
  T& x() { return this->values[0]; }
  T& y() { return this->values[1]; }
  T& z() { return this->values[2]; }
  T x() const { return this->values[0]; }
  T y() const { return this->values[1]; }
  T z() const { return this->values[2]; }
};

template<typename T>
class Vec4 : public Vec<T, 4> {
public:
  T& x() { return this->values[0]; }
  T& y() { return this->values[1]; }
  T& z() { return this->values[2]; }
  T& w() { return this->values[3]; }
  T x() const { return this->values[0]; }
  T y() const { return this->values[1]; }
  T z() const { return this->values[2]; }
  T w() const { return this->values[3]; }
};

using Vec2f = Vec2<float>;
using Vec3f = Vec3<float>;
using Vec4f = Vec4<float>;

template<typename T, int dim>
inline auto operator+(const Vec<T, dim>& a, const Vec<T, dim>& b) -> Vec<T, dim> {
  Vec<T, dim> result;
  for (auto i = 0; i < dim; ++i) {
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
  for (auto i = 0; i < dim; ++i) {
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
  for (auto i = 0; i < dim; ++i) {
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
  for (auto i = 0; i < dim; ++i) {
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
  for (auto i = 0; i < dim; ++i) {
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
  for (auto i = 0; i < dim; ++i) {
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
  for (auto i = 0; i < dim; ++i) {
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
  for (int i = 0; i < dim; ++i) {
    result[i] = v[i] / len;
  }
  return result;
}

template<typename T>
inline auto cross(const Vec3<T>& a, const Vec3<T>& b) -> Vec3<T> {
  return Vec3<T>{
    a.y() * b.z() - a.z() * b.y(),
    a.z() * b.x() - a.x() * b.z(),
    a.x() * b.y() - a.y() * b.x()
  };
}

#endif // VECTOR_HPP