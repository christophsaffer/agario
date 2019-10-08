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
