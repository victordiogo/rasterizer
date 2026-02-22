#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "math/conversions.hpp"
#include "math/vector.hpp"
#include "math/matrix.hpp"
#include <stdexcept>

enum class Movement {
  forward,
  backward,
  left,
  right,
  up,
  down
};

class Camera {
public:
  Vec3f position;

  Camera(const Vec3f& position, float sensitivity, float fovy, float aspect, float near = 0.1f)
    : position{position},
      m_pitch{0.0f},
      m_yaw{0.0f},
      m_sensitivity{sensitivity},
      m_fovy{fovy},
      m_aspect{aspect},
      m_near{near}
  {
    if (m_pitch > 89.9f || m_pitch < -89.9f)
      throw std::out_of_range{"Camera pitch must be between -89.9 and 89.9 degrees"};
    
    if (m_sensitivity <= 0.0f)
      throw std::out_of_range{"Camera sensitivity must be greater than 0"};

    if (m_fovy <= 0.0f || m_fovy >= 180.0f)
      throw std::out_of_range{"Camera vertical FOV must be between 0 and 180 degrees"};

    if (m_aspect <= 0.0f)
      throw std::out_of_range{"Camera aspect ratio must be greater than 0"};
    
    if (m_near <= 0.0f)
      throw std::out_of_range{"Camera near plane must be greater than 0"};

    update_vectors(); // Initialize front, right, and up vectors
  }

  auto view_matrix() const -> Mat4f {
    //return glm::lookAt(position, position + m_front, m_up);
  }

  // infinite reversed-z projection matrix
  auto projection_matrix() const -> Mat4f {
    auto f = 1.0f / std::tan(radians(m_fovy) / 2.0f);
    auto mat = Mat4f{};
    mat[0][0] = f / m_aspect;
    mat[1][1] = f;
    mat[3][2] = m_near;
    mat[2][3] = -1.0f;
    return mat;
  }

  auto front() const -> const Vec3f& {
    return m_front;
  }

  auto right() const -> const Vec3f& {
    return m_right;
  }

  auto up() const -> const Vec3f& {
    return m_up;
  }

  auto near() const -> float {
    return m_near;
  }

  auto fovy() const -> float {
    return m_fovy;
  }

  auto aspect() const -> float {
    return m_aspect;
  }

  auto set_aspect(float aspect) -> void {
    if (aspect <= 0.0f)
      throw std::out_of_range{"Camera aspect ratio must be greater than 0"};

    m_aspect = aspect;
  }
  
  auto move(Movement direction, float velocity) -> void {
    if (direction == Movement::forward)
      position += normalize(Vec3f{m_front.x, 0.0f, m_front.z}) * velocity;
    if (direction == Movement::backward)
      position -= normalize(Vec3f{m_front.x, 0.0f, m_front.z}) * velocity;
    if (direction == Movement::left)
      position -= m_right * velocity;
    if (direction == Movement::right)
      position += m_right * velocity;
    if (direction == Movement::up)
      position += Vec3f{0.0f, 1.0f, 0.0f} * velocity;
    if (direction == Movement::down)
      position -= Vec3f{0.0f, 1.0f, 0.0f} * velocity;
  }
  
  auto rotate(const Vec2f& offset) -> void {
    m_yaw += offset.x * m_sensitivity;
    m_pitch += offset.y * m_sensitivity;
    
    if (m_pitch > 89.9f) m_pitch = 89.9f;
    if (m_pitch < -89.9f) m_pitch = -89.9f;
    
    update_vectors();
  }
  
private:
  Vec3f m_front;
  Vec3f m_up;
  Vec3f m_right;
  float m_pitch; // angle in degrees around the Right axis (0 means looking at the horizon, positive means looking up)
  float m_yaw; // angle in degrees around the Y axis (0 means looking -Z, positive means looking to the right)
  float m_sensitivity;
  float m_fovy; // in degrees
  float m_aspect;
  float m_near;

  auto update_vectors() -> void {
    m_front.x = std::sin(radians(m_yaw)) * std::cos(radians(m_pitch));
    m_front.y = std::sin(radians(m_pitch));
    m_front.z = -std::cos(radians(m_yaw)) * std::cos(radians(m_pitch));
    m_front = normalize(m_front);

    m_right = normalize(cross(m_front, {0.0f, 1.0f, 0.0f}));
    m_up = normalize(cross(m_right, m_front));
  }
};

#endif // CAMERA_HPP