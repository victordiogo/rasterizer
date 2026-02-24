#ifndef GL_SHADER_HPP
#define GL_SHADER_HPP

#include <glad/gl.h>
#include <string>
#include <fstream>
#include <optional>
#include <utility>
#include <stdexcept>

namespace gl {

inline auto get_file_content(std::ifstream& file) -> std::string {
  file.seekg(0, std::ios::end);
  auto contents = std::string{};
  contents.resize((std::size_t)file.tellg());
  file.seekg(0, std::ios::beg);
  file.read(contents.data(), (std::streamsize)contents.size());
  return contents;
}

// parameter Shader is true if the id is a Shader id or false if it's a program id
// returns the error message if there was an error, or none if there was no error
inline auto check_errors(unsigned id, bool Shader) -> std::optional<std::string> {
  auto success = 0;
  if (Shader) {
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  } else {
    glGetProgramiv(id, GL_LINK_STATUS, &success);
  }
  if (success == 1) return {};

  auto info = std::string(1024, '\0');
  if (Shader) {
    glGetShaderInfoLog(id, 1024, nullptr, info.data());
  } else {
    glGetProgramInfoLog(id, 1024, nullptr, info.data());
  }
  
  return info;
}

inline auto create_shader(unsigned type, const char* source) -> unsigned {
  auto Shader = glCreateShader(type);
  glShaderSource(Shader, 1, &source, nullptr);
  glCompileShader(Shader);
  return Shader;
}

class Shader {
public:
  Shader(const std::string& vertex_path, const std::string& fragment_path) : m_id{0u} {
    auto vertex_file = std::ifstream{vertex_path};
    if (!vertex_file) 
      throw std::runtime_error{"Could not open vertex Shader file: " + vertex_path};

    auto fragment_file = std::ifstream{fragment_path};
    if (!fragment_file)
      throw std::runtime_error{"Could not open fragment Shader file: " + fragment_path};
    
    auto vertex_content = get_file_content(vertex_file);
    auto fragment_content = get_file_content(fragment_file);
    
    auto vertex = create_shader(GL_VERTEX_SHADER, vertex_content.c_str());
    if (auto error = check_errors(vertex, true)) {
      glDeleteShader(vertex);
      throw std::runtime_error{"Error when compiling vertex Shader file " + vertex_path + ":\n" + *error};
    }
    
    auto fragment = create_shader(GL_FRAGMENT_SHADER, fragment_content.c_str());
    if (auto error = check_errors(fragment, true)) {
      glDeleteShader(vertex);
      glDeleteShader(fragment);
      throw std::runtime_error{"Error when compiling fragment Shader file " + fragment_path + ":\n" + *error};
    }

    m_id = glCreateProgram();
    glAttachShader(m_id, vertex);
    glAttachShader(m_id, fragment);
    glLinkProgram(m_id);
    if (auto error = check_errors(m_id, false)) {
      glDeleteShader(vertex);
      glDeleteShader(fragment);
      glDeleteProgram(m_id);
      throw std::runtime_error{"Error when linking Shader program: " + *error};
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
  }
  
  Shader(Shader&& other) noexcept : m_id{other.m_id} {
    other.m_id = 0u;
  }

  Shader(const Shader&) = delete;

  ~Shader() {
    if (m_id != 0u) glDeleteProgram(m_id);
  }

  auto operator=(const Shader&) -> Shader& = delete;

  auto operator=(Shader&& other) noexcept -> Shader& {
    std::swap(m_id, other.m_id);
    return *this;
  }

  auto id() const -> unsigned {
    return m_id;
  }

  auto use() const -> void {
    glUseProgram(m_id);
  }

  auto set_uniform(const std::string& name, int value) const -> void {
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
  }

  auto set_uniform(const std::string& name, float value) const -> void {
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
  }

private:
  unsigned m_id;
};

} // namespace gl

#endif // GL_SHADER_HPP