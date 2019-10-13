#pragma once

#include <array>
#include <cmath>
#include <vector>

class player {
 public:
  player(){};
  player(float s, std::array<float, 2> p) : circle_sizes{s}, origin{p} {};

  ~player() = default;

 public:
  void split_circles(std::vector<float>& v);
  std::array<float, 2> compute_velocity(const float& mouse_x,
                                        const float& mouse_y,
                                        const float& screen_width,
                                        const float& screen_height);

 public:
  float max_velocity{200};
  std::array<float, 2> origin{0, 0};
  std::array<float, 2> velocity{0, 0};
  std::vector<float> circle_sizes = {0.05};
};