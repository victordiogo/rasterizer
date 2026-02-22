#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>

class Timer {
public:
  auto reset() -> void { m_begin = Clock::now(); }

  // Returns elapsed time in seconds
  auto elapsed() const -> double {
    return std::chrono::duration_cast<Duration>(Clock::now() - m_begin).count();
  }

private:
  using Clock = std::chrono::steady_clock;
  using Duration = std::chrono::duration<double>;
  std::chrono::time_point<Clock> m_begin{ Clock::now() };
};

#endif // TIMER_HPP