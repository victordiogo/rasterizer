#ifndef GL_BUFFER_HPP
#define GL_BUFFER_HPP

#include <glad/gl.h>
#include <utility>

namespace gl {
// Buffer Object RAII wrapper
class Buffer {
public:
  Buffer() {
    glGenBuffers(1, &m_id);
  }

  Buffer(const Buffer&) = delete;

  Buffer(Buffer&& other) noexcept : m_id{other.m_id} {
    other.m_id = 0u;
  }

  ~Buffer() {
    if (m_id != 0u) glDeleteBuffers(1, &m_id);
  }

  auto operator=(const Buffer&) -> Buffer& = delete;

  auto operator=(Buffer&& other) noexcept -> Buffer& {
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

#endif // GL_BUFFER_HPP