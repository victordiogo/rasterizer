#ifndef GL_VERTEX_ARRAY_HPP
#define GL_VERTEX_ARRAY_HPP

#include <glad/gl.h>
#include <utility>

namespace gl {

// Vertex Array RAII wrapper
class VertexArray {
public:
  VertexArray() {
    glGenVertexArrays(1, &m_id);
  }

  VertexArray(const VertexArray&) = delete;

  VertexArray(VertexArray&& other) noexcept : m_id{other.m_id} {
    other.m_id = 0u;
  }

  ~VertexArray() {
    if (m_id != 0u) glDeleteVertexArrays(1, &m_id);
  }

  auto operator=(const VertexArray&) -> VertexArray& = delete;

  auto operator=(VertexArray&& other) noexcept -> VertexArray& {
    std::swap(m_id, other.m_id);
    return *this;
  }

  auto id() const -> unsigned {
    return m_id;
  }
  
private:
  unsigned m_id;
};

} // namespace gl

#endif // GL_VERTEX_ARRAY_HPP 