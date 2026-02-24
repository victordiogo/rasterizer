#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "camera.hpp"
#include "math/vector.hpp"
#include "math/matrix.hpp"
#include "model/model.hpp"
#include <vector>
#include <cstdint>
#include <cassert>

class Renderer {
public:
  Renderer(int width, int height) 
  : m_width{width}, 
    m_height{height},
    m_colorbuffer((unsigned)(width * height), 0)
  {}

  auto set_color(int x, int y, const Vec4f& color) -> void {
    assert(x >= 0 && x < m_width);
    assert(y >= 0 && y < m_height);
    auto index = (unsigned)(y * m_width + x);
    m_colorbuffer[index] = (std::uint32_t)(color.r * 255.5f) << 24 |
                           (std::uint32_t)(color.g * 255.5f) << 16 |
                           (std::uint32_t)(color.b * 255.5f) << 8 |
                           (std::uint32_t)(color.a * 255.5f);
  }

  auto resize(int width, int height) -> void {
    m_width = width;
    m_height = height;
    m_colorbuffer.resize((unsigned)(width * height), 0);
  }

  auto render(const Camera& camera, const Model& model) -> void {
    auto view = camera.view_matrix();
    auto projection = camera.projection_matrix();

    std::fill(m_colorbuffer.begin(), m_colorbuffer.end(), 0); // Clear to black

    for (const auto& mesh : model.meshes()) {
      for (auto i = 0u; i < mesh.vertices.size(); i += 3) {
        const auto& v0 = mesh.vertices[i + 0];
        const auto& v1 = mesh.vertices[i + 1];
        const auto& v2 = mesh.vertices[i + 2];

        auto pos0_clip = Vec4f{v0.position, 1.0f} * view * projection;
        auto pos1_clip = Vec4f{v1.position, 1.0f} * view * projection;
        auto pos2_clip = Vec4f{v2.position, 1.0f} * view * projection;

        // perform clipping

        auto pos0_ndc = pos0_clip.xyz() / pos0_clip.w;
        auto pos1_ndc = pos1_clip.xyz() / pos1_clip.w;
        auto pos2_ndc = pos2_clip.xyz() / pos2_clip.w;

        auto pos0_screen = (pos0_ndc + Vec3f{1.0f}) * 0.5f;
        auto pos1_screen = (pos1_ndc + Vec3f{1.0f}) * 0.5f;
        auto pos2_screen = (pos2_ndc + Vec3f{1.0f}) * 0.5f;
        pos0_screen.x *= m_width - 1;
        pos0_screen.y = (1.0f - pos0_screen.y) * (m_height - 1);
        pos1_screen.x *= m_width - 1;
        pos1_screen.y = (1.0f - pos1_screen.y) * (m_height - 1);
        pos2_screen.x *= m_width - 1;
        pos2_screen.y = (1.0f - pos2_screen.y) * (m_height - 1);

        auto draw_line = [&](const Vec3f& p0, const Vec3f& p1) {
          // Bresenham's line algorithm
          auto x0 = (int)std::round(p0.x);
          auto y0 = (int)std::round(p0.y);
          auto x1 = (int)std::round(p1.x);
          auto y1 = (int)std::round(p1.y);

          auto dx = std::abs(x1 - x0);
          auto dy = std::abs(y1 - y0);
          auto sx = x0 < x1 ? 1 : -1;
          auto sy = y0 < y1 ? 1 : -1;
          auto err = dx - dy;

          while (true) {
            if (x0 >= 0 && x0 < m_width && y0 >= 0 && y0 < m_height)
              set_color(x0, y0, Vec4f{mesh.material.diffuse, 1.0f});
            if (x0 == x1 && y0 == y1) break;
            auto err2 = err * 2;
            if (err2 > -dy) {
              err -= dy;
              x0 += sx;
            }
            if (err2 < dx) {
              err += dx;
              y0 += sy;
            }
          }
        };
        draw_line(pos0_screen, pos1_screen);
        draw_line(pos1_screen, pos2_screen);
        draw_line(pos2_screen, pos0_screen);
      }
    }
  }

  auto colorbuffer() const -> const std::vector<std::uint32_t>& {
    return m_colorbuffer;
  }

private:
  int m_width;
  int m_height;
  std::vector<std::uint32_t> m_colorbuffer; // RGBA
};

#endif // RENDERER_HPP