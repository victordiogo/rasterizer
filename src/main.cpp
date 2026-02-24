#include "camera.hpp"
#include "frame-monitor.hpp"
#include "frame-presenter.hpp"
#include "window/window.hpp"
#include "window/glfw-guard.hpp"
#include "model/model.hpp"
#include "renderer.hpp"
#include <print>

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

auto on_cursor_pos(const Vec2f& pos, Vec2f& last_pos, bool& reset, Camera& camera) -> void {
  if (glfwGetInputMode(glfwGetCurrentContext(), GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
    return;

  if (reset) {
    last_pos = pos;
    reset = false;
    return;
  }

  auto offset = pos - last_pos;
  last_pos = pos;

  constexpr auto sensitivity = 0.1f;
  camera.rotate(offset.x * sensitivity, -offset.y * sensitivity);
}

auto process_input(double frame_time, Camera& camera) -> void {
  auto speed = 2.0f * (float)frame_time;
  if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W) == GLFW_PRESS)
    camera.move(Movement::forward, speed);
  if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S) == GLFW_PRESS)
    camera.move(Movement::backward, speed);
  if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A) == GLFW_PRESS)
    camera.move(Movement::left, speed);
  if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_D) == GLFW_PRESS)
    camera.move(Movement::right, speed);
  if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_SPACE) == GLFW_PRESS)
    camera.move(Movement::up, speed);
  if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    camera.move(Movement::down, speed);
}

auto main() -> int {
  auto guard = GlfwGuard{};
  auto window = create_window(800, 600);
  glfwSwapInterval(0);

  init_glad();

  int width, height;
  glfwGetFramebufferSize(window.get(), &width, &height);
  glViewport(0, 0, width, height);

  auto renderer = Renderer{width, height};
  auto frame_presenter = FramePresenter{width, height};
  auto camera = Camera{{0.0f, 0.0f, 5.0f}, 60.0f, (float)width / height};
  auto model = Model{"../resources/assets/teapot.obj"};

  std::println("Camera pos: {} {} {}", camera.position.x, camera.position.y, camera.position.z);
  std::println("Camera front: {} {} {}", camera.front().x, camera.front().y, camera.front().z);
  std::println("Camera right: {} {} {}", camera.right().x, camera.right().y, camera.right().z);
  std::println("Camera up : {} {} {}", camera.up().x, camera.up().y, camera.up().z);

  auto v = Vec4f{0.0f, 0.0f, 0.0f, 1.0f};
  v = v * camera.view_matrix();
  std::println("v : {} {} {}", v.x, v.y, v.z);

  window.set_framebuffer_size_callback([&](GLFWwindow*, int width, int height) {
    on_framebuffer_size(width, height, frame_presenter);
  });

  auto last_cursor = Vec2f{-1.0f, -1.0f};
  auto reset_cursor = true;
  window.set_cursor_pos_callback([&](GLFWwindow*, double xpos, double ypos) {
    on_cursor_pos(Vec2f{(float)xpos, (float)ypos}, last_cursor, reset_cursor, camera);
  });

  window.set_key_callback([&](GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  });

  window.set_mouse_button_callback([&](GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      reset_cursor = true;
    }
  });

  auto frame_monitor = FrameMonitor{1.0};

  while (!glfwWindowShouldClose(window.get())) {
    frame_monitor.update();
    glfwSetWindowTitle(window.get(), std::format("Rasterizer - {:.0f} FPS", frame_monitor.fps()).c_str());

    glfwPollEvents();
    process_input(frame_monitor.frame_time(), camera);

    renderer.render(camera, model);
    frame_presenter.present(renderer.colorbuffer());


    glfwSwapBuffers(window.get());
  }

  return 0;
}