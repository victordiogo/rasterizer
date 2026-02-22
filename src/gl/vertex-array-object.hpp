#ifndef VERTEX_ARRAY_OBJECT_HPP
#define VERTEX_ARRAY_OBJECT_HPP

#include <glad/gl.h>
#include <utility>

// Vertex Array Object RAII wrapper
class Vao {
public:
  Vao() {
    glGenVertexArrays(1, &m_id);
  }

  Vao(const Vao&) = delete;

  Vao(Vao&& other) noexcept : m_id{other.m_id} {
    other.m_id = 0u;
  }

  ~Vao() {
    if (m_id != 0u) glDeleteVertexArrays(1, &m_id);
  }

  auto operator=(const Vao&) -> Vao& = delete;

  auto operator=(Vao&& other) noexcept -> Vao& {
    std::swap(m_id, other.m_id);
    return *this;
  }

  auto id() const -> unsigned {
    return m_id;
  }
  
private:
  unsigned m_id;
};

#endif // VERTEX_ARRAY_OBJECT_HPP 