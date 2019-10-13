#include <player.hpp>

//#include <array>
//#include <cmath>

std::array<float, 2> player::compute_velocity(const float& mouse_x,
                                              const float& mouse_y,
                                              const float& screen_width,
                                              const float& screen_height) {
  velocity[0] = mouse_x - screen_width / 2;
  velocity[1] = mouse_y - screen_height / 2;
  if (abs(velocity[0]) > max_velocity) {
    velocity[0] = max_velocity * velocity[0] / abs(velocity[0]);
  }
  if (abs(velocity[1]) > max_velocity) {
    velocity[1] = max_velocity * velocity[1] / abs(velocity[1]);
  }
  return velocity;
}