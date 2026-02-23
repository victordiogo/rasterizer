#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <GLFW/glfw3.h>
#include <functional>

// Wrapper for GLFWwindow with RAII semantics and callback management
// Glfw must be initialized before creating a Window object
class Window {
public:
  Window(int width, int height, const char* title);
  Window(const Window&) = delete;
  Window(Window&& other) noexcept;

  ~Window() {
    if (m_handle) glfwDestroyWindow(m_handle);
  }

  auto operator=(const Window&) -> Window& = delete;
  auto operator=(Window&& other) noexcept -> Window&;

  auto get() const -> GLFWwindow* { 
    return m_handle; 
  }

  // Parameters: window, width, height
  auto set_framebuffer_size_callback(std::function<void(GLFWwindow*, int, int)> callback) -> void {
    m_framebuffer_size_callback = std::move(callback);
  }

  // Parameters: window, current cursor pos
  auto set_cursor_pos_callback(std::function<void(GLFWwindow*, double, double)> callback) -> void {
    m_cursor_pos_callback = std::move(callback);
  }

  // Parameters: window, button, action, mods
  auto set_mouse_button_callback(std::function<void(GLFWwindow*, int, int, int)> callback) -> void {
    m_mouse_button_callback = std::move(callback);
  }

  // Parameters: window, key, scancode, action, mods
  auto set_key_callback(std::function<void(GLFWwindow*, int, int, int, int)> callback) -> void {
    m_key_callback = std::move(callback);
  }

private:
  GLFWwindow* m_handle;
  std::function<void(GLFWwindow*, int, int)> m_framebuffer_size_callback;
  std::function<void(GLFWwindow*, double, double)> m_cursor_pos_callback;
  std::function<void(GLFWwindow*, int, int, int)> m_mouse_button_callback;
  std::function<void(GLFWwindow*, int, int, int, int)> m_key_callback;

  static auto framebuffer_size_callback(GLFWwindow* window, int width, int height) -> void;
  static auto cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) -> void;
  static auto mouse_button_callback(GLFWwindow* window, int button, int action, int mods) -> void;
  static auto key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) -> void;
};

#endif