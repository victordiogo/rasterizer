#include "window/glfw-guard.hpp"
#include "window/window.hpp"
#include "frame-monitor.hpp"
#include <print>

auto create_window(int width, int height) -> Window {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  return Window{width, height, "Rasterizer"};
}

auto main() -> int {
  auto guard = GlfwGuard{};
  auto window = create_window(800, 600);

  glfwSwapInterval(0);

  auto frame_monitor = FrameMonitor{1.0};

  while (!glfwWindowShouldClose(window.get())) {
    frame_monitor.update();
    glfwSetWindowTitle(window.get(), std::format("Rasterizer - {:.0f} FPS", frame_monitor.fps()).c_str());

    glfwPollEvents();
    glfwSwapBuffers(window.get());
  }

  return 0;
}