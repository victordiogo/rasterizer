#include "window/window.hpp"
#include <stdexcept>
#include <utility>

Window::Window(int width, int height, const char* title) {
  m_handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
  if (!m_handle)
    throw std::runtime_error{"Failed to create GLFW window"};

  glfwMakeContextCurrent(m_handle);
  glfwSetWindowUserPointer(m_handle, this);
  // callbacks
  glfwSetFramebufferSizeCallback(m_handle, framebuffer_size_callback);
  glfwSetCursorPosCallback(m_handle, cursor_pos_callback);
  glfwSetMouseButtonCallback(m_handle, mouse_button_callback);
  glfwSetKeyCallback(m_handle, key_callback);
}

Window::Window(Window&& other) noexcept : 
  m_handle{other.m_handle},
  m_framebuffer_size_callback{std::move(other.m_framebuffer_size_callback)},
  m_cursor_pos_callback{std::move(other.m_cursor_pos_callback)},
  m_mouse_button_callback{std::move(other.m_mouse_button_callback)},
  m_key_callback{std::move(other.m_key_callback)}
{
  other.m_handle = nullptr;
}

auto Window::operator=(Window&& other) noexcept -> Window& {
  std::swap(m_handle, other.m_handle);
  std::swap(m_framebuffer_size_callback, other.m_framebuffer_size_callback);
  std::swap(m_cursor_pos_callback, other.m_cursor_pos_callback);
  std::swap(m_mouse_button_callback, other.m_mouse_button_callback);
  std::swap(m_key_callback, other.m_key_callback);
  return *this;
}
  
auto Window::framebuffer_size_callback(GLFWwindow* window, int width, int height) -> void {
  auto* ptr = (Window*)glfwGetWindowUserPointer(window);
  if (ptr->m_framebuffer_size_callback)
    ptr->m_framebuffer_size_callback(window, width, height);
}

auto Window::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) -> void {
  auto* ptr = (Window*)glfwGetWindowUserPointer(window);
  if (ptr->m_cursor_pos_callback)
    ptr->m_cursor_pos_callback(window, xpos, ypos);
}

auto Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) -> void {
  auto* ptr = (Window*)glfwGetWindowUserPointer(window);
  if (ptr->m_mouse_button_callback)
    ptr->m_mouse_button_callback(window, button, action, mods);
}

auto Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) -> void {
  auto* ptr = (Window*)glfwGetWindowUserPointer(window);
  if (ptr->m_key_callback)
    ptr->m_key_callback(window, key, scancode, action, mods);
}