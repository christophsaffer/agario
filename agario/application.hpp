#pragma once
#include <SFML/Graphics.hpp>
#include <vector2.hpp>

#include <player.hpp>

class application {
 public:
  application() { resize(); }
  application(int w, int h);
  ~application() = default;

  void execute();

 private:
  void resize();
  void resize(int w, int h);
  void compute_viewport();
  std::array<float, 2> compute_pixel(float real_x, float real_y);
  float scale_length(const float distance);

 private:
  int screen_width;
  int screen_height;

  // viewport
  me::vector2f origin{};
  me::vector2f view_dim =
      2.0f * me::vector2f{1, static_cast<float>(screen_width) / screen_height};
  me::vector2f view_min;
  me::vector2f view_max;

  // player instance
  player player1{};

  // scale factor
  float scale_factor;

  // rendering
  sf::RenderWindow window{sf::VideoMode(screen_width, screen_height),
                          "Fractal Render"};
  bool update = true;
};
