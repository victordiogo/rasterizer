#include <glad/gl.h>
#include <print>
#include "window/glfw-guard.hpp"
#include "window/window.hpp"
#include "frame-monitor.hpp"

#include <stdexcept>
#include <array>

#include "math/vector.hpp"
#include "math/matrix.hpp"
#include "timer.hpp"
#include "frame-presenter.hpp"

auto create_window(int width, int height) -> Window {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  return Window{width, height, "Rasterizer"};
}

auto init_glad() -> void {
  auto version = gladLoadGL(glfwGetProcAddress);
  if (version == 0)
    throw std::runtime_error{"Failed to initialize GLAD"};
}

auto on_framebuffer_size(int width, int height, FramePresenter& presenter) -> void {
  glViewport(0, 0, width, height);
  presenter.resize(width, height);
}

auto gen_frame(std::vector<unsigned char>& buffer, int width, int height) -> void {
  static auto s_frame = 0u;
  for (auto y = 0u; y < (unsigned)height; ++y) {
    for (auto x = 0u; x < (unsigned)width; ++x) {
      auto index = (y * (unsigned)width + x) * 4u;
      buffer[index + 0] = (unsigned char)(x  + s_frame % 256u); // R
      buffer[index + 1] = (unsigned char)(y  + s_frame % 256u); // G
      buffer[index + 2] = 0; // B
      buffer[index + 3] = 255; // A
    }
  }
  ++s_frame;
}

auto main() -> int {
  auto guard = GlfwGuard{};
  auto window = create_window(800, 600);
  glfwSwapInterval(0);

  init_glad();

  int width, height;
  glfwGetFramebufferSize(window.get(), &width, &height);
  glViewport(0, 0, width, height);

  auto frame_presenter = FramePresenter{width, height};
  window.set_framebuffer_size_callback([&](GLFWwindow*, int width, int height) {
    on_framebuffer_size(width, height, frame_presenter);
  });

  std::vector<unsigned char> colorbuffer((unsigned)(width * height * 4));

  auto frame_monitor = FrameMonitor{1.0};

  while (!glfwWindowShouldClose(window.get())) {
    frame_monitor.update();
    glfwSetWindowTitle(window.get(), std::format("Rasterizer - {:.0f} FPS", frame_monitor.fps()).c_str());

    gen_frame(colorbuffer, width, height);

    frame_presenter.present(colorbuffer);

    glfwPollEvents();
    glfwSwapBuffers(window.get());
  }

  return 0;
}