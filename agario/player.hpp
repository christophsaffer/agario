#pragma once

#include <array>
#include <cmath>
#include <vector>

class player {
 public:
  player(){};
  player(float s, std::array<float, 2> p) : size_circles{s}, position{p} {};

  ~player() = default;

 public:
  void split_circles(std::vector<float>& v);
  std::array<float, 2> compute_velocity(const float& mouse_x,
                                        const float& mouse_y,
                                        const float& screen_width,
                                        const float& screen_height);

  void set_name(const std::string& n) { name = n; }

  bool player_alive(bool a) {
    alive = a;
    return alive;
  }

 public:
  float max_velocity{200};
  std::array<float, 2> position{0, 0};
  std::array<float, 2> velocity{0, 0};
  std::vector<float> size_circles = {0.05};

 private:
  std::string name = "nameless";
  bool alive = true;
};