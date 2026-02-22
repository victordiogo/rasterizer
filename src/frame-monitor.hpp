#ifndef FRAME_MONITOR_HPP
#define FRAME_MONITOR_HPP

#include "timer.hpp"
#include <cstdint>
#include <stdexcept>

// Calculates frames per second (FPS) and frame time.
class FrameMonitor {
public:
  // update_interval is in seconds
  explicit FrameMonitor(double fps_update_interval = 1.0)
    : m_frame_time{0.0},
      m_fps{0.0}, 
      m_fps_update_interval{fps_update_interval},
      m_last_fps_update{0.0},
      m_frames{0},
      m_last_update{0.0}
  {
    if (fps_update_interval <= 0.0)
      throw std::invalid_argument("update_interval must be positive");
  }

  // should be called on start of each frame
  auto update() -> void {
    auto current = m_timer.elapsed();
    m_frame_time = current - m_last_update;
    m_last_update = current;

    if (current - m_last_fps_update >= m_fps_update_interval) {
      m_fps = (double)m_frames / (current - m_last_fps_update);
      m_last_fps_update = current;
      m_frames = 0;
    }

    ++m_frames;
  }

  auto frame_time() const -> double {
    return m_frame_time;
  }

  auto fps() const -> double {
    return m_fps;
  }

private:
  double m_frame_time;
  double m_fps;
  double m_fps_update_interval;
  double m_last_fps_update;
  std::uint64_t m_frames; // since last update
  double m_last_update;
  Timer m_timer;
};

#endif // FRAME_MONITOR_HPP