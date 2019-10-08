#pragma once
#include <SFML/Graphics.hpp>
#include <vector2.hpp>

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

private:
  int screen_width = 500;
  int screen_height = 500;

  // viewport
  me::vector2f origin{};
  me::vector2f view_dim =
      2.0f * me::vector2f{1, static_cast<float>(screen_width) / screen_height};
  me::vector2f view_min;
  me::vector2f view_max;

  // rendering
  sf::RenderWindow window{sf::VideoMode(screen_width, screen_height),
                          "Fractal Render"};
  bool update = true;
};
