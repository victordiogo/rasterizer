#ifndef FRAME_PRESENTER_HPP
#define FRAME_PRESENTER_HPP

#include "gl/buffer.hpp"
#include "gl/texture.hpp"
#include "gl/vertex-array.hpp"
#include "gl/shader.hpp"
#include <vector>
#include <cstdint>

#include <cassert>

class FramePresenter {
public:
  FramePresenter(int width, int height) 
  : m_width{width},
    m_height{height},
    m_shader{"../resources/shaders/screen-quad.vert", "../resources/shaders/screen-quad.frag"} 
  {
    if (width <= 0 || height <= 0)
      throw std::runtime_error{"Width and height must be positive and non-zero"};
     
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbo.id());
    glBufferData(GL_PIXEL_UNPACK_BUFFER, (unsigned)(width * height) * sizeof(std::uint32_t), nullptr, GL_STREAM_DRAW);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    glBindTexture(GL_TEXTURE_2D, m_texture.id());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    auto vertices = std::array{
      -1.0f, -1.0f, 0.0f, 1.0f,
      1.0f, -1.0f, 1.0f, 1.0f,
      1.0f,  1.0f, 1.0f, 0.0f,

      1.0f,  1.0f, 1.0f, 0.0f,
      -1.0f,  1.0f, 0.0f, 0.0f,
      -1.0f, -1.0f, 0.0f, 1.0f
    };

    glBindVertexArray(m_vao.id());
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo.id());
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
  }

  // frame size must match this presenter's size
  auto present(const std::vector<std::uint32_t>& frame) -> void {
    assert(frame.size() == (unsigned)(m_width * m_height));

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbo.id());

    auto* ptr = (std::uint32_t*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
    std::copy(frame.data(), frame.data() + frame.size(), ptr);
    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

    glBindTexture(GL_TEXTURE_2D, m_texture.id());
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, 0);

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    m_shader.use();
    glBindTexture(GL_TEXTURE_2D, m_texture.id());
    glBindVertexArray(m_vao.id());
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
  }

  auto resize(int width, int height) -> void {
    if (width <= 0 || height <= 0)
      throw std::runtime_error{"Width and height must be positive and non-zero"};

    m_width = width;
    m_height = height;

    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbo.id());
    glBufferData(GL_PIXEL_UNPACK_BUFFER, (unsigned)(width * height) * sizeof(std::uint32_t), nullptr, GL_STREAM_DRAW);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    glBindTexture(GL_TEXTURE_2D, m_texture.id());
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
  }

private:
  int m_width;
  int m_height;
  gl::Buffer m_pbo;
  gl::Texture m_texture;
  gl::VertexArray m_vao;
  gl::Buffer m_vbo;
  gl::Shader m_shader;
};

#endif // FRAME_PRESENTER_HPP