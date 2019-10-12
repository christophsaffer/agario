#include <application.hpp>

application::application(int w, int h) : screen_width{w}, screen_height{h} {
  resize();
}

void application::resize() {
  window.setView(sf::View{sf::FloatRect{0, 0, static_cast<float>(screen_width),
                                        static_cast<float>(screen_height)}});
}

void application::resize(int w, int h) {
  screen_width = w;
  screen_height = h;
  resize();
}

void application::compute_viewport() {
  view_dim.x = view_dim.y * screen_width / screen_height;
  view_min = origin - 0.5f * view_dim;
  view_max = origin + 0.5f * view_dim;
}

std::array<float, 2> application::compute_pixel(float real_x, float real_y) {

  std::array<float, 2> pixel;
  pixel[0] = (real_x - view_min.x) / (view_max.x - view_min.x) * screen_width;
  pixel[1] = (real_y - view_min.y) / (view_max.y - view_min.y) * screen_height;

  return pixel;
}

float application::scale_length(float distance) {
  return distance * (-screen_height / (view_max.y - view_min.y));
}
