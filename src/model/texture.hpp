#ifndef MODEL_TEXTURE_HPP
#define MODEL_TEXTURE_HPP

#include "math/vector.hpp"
#include <string>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <stb_image.h>

class Texture {
public:
  explicit Texture(const std::string& filepath, bool vertical_flip = true) {
    stbi_set_flip_vertically_on_load(vertical_flip);
    auto channels = 0;
    auto* data = stbi_load(filepath.c_str(), &m_width, &m_height, &channels, 0);
    if (!data)
      throw std::runtime_error{"Failed to load texture: " + filepath};

    m_data.reserve((unsigned)(m_width * m_height));
    for (auto i = 0u; i < (unsigned)(m_width * m_height); ++i) {
      auto ch = (unsigned)channels;
      auto r = data[i * ch + 0] / 255.0f;
      auto g = data[i * ch + 1] / 255.0f;
      auto b = data[i * ch + 2] / 255.0f;
      auto a = ch >= 4 ? data[i * ch + 3] / 255.0f : 1.0f;
      m_data.emplace_back(r, g, b, a);
    }
    
    stbi_image_free(data);
  }

  // nearest sampling with repeat wrapping
  auto operator[](Vec2f uv) const -> const Vec4f& {
    uv.x -= std::floor(uv.x); // range is now [0, 1)
    uv.y -= std::floor(uv.y);
    auto x = (unsigned)(uv.x * m_width);
    auto y = (unsigned)(uv.y * m_height);
    return m_data[y * (unsigned)m_width + x];
  }

private:
  int m_width;
  int m_height;
  std::vector<Vec4f> m_data; // RGBA normalized to [0, 1]
};

#endif // MODEL_TEXTURE_HPP