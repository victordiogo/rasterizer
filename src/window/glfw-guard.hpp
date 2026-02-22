#ifndef GLFW_GUARD_HPP
#define GLFW_GUARD_HPP

#include <GLFW/glfw3.h>
#include <stdexcept>

// RAII wrapper for GLFW initialization and termination
class GlfwGuard {
public:
  GlfwGuard() {
    if (s_initialized)
      throw std::logic_error{"GLFW already initialized"};

    if (glfwInit() == GLFW_FALSE)
      throw std::runtime_error{"Failed to initialize GLFW"};
    
    s_initialized = true;
  }

  GlfwGuard(const GlfwGuard&) = delete;
  GlfwGuard(GlfwGuard&&) = delete;

  ~GlfwGuard() {
    glfwTerminate();
    s_initialized = false;
  }

  auto operator=(const GlfwGuard&) -> GlfwGuard& = delete;
  auto operator=(GlfwGuard&&) -> GlfwGuard& = delete;
  
private:
  inline static bool s_initialized = false;
};

#endif // GLFW_GUARD_HPP