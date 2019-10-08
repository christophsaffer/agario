#include <application.hpp>

#include <cmath>
#include <iostream>

void application::execute() {
  using namespace std;

  constexpr float speed = 1e-2;
  constexpr int num_grid_lines = 30;

  float pos_x = 0;
  float pos_y = 0;

  int old_mouse_x = 0;
  int old_mouse_y = 0;

  std::vector<float> grid;
  float start_pos_x = -10;
  float end_pos_x = 10;
  float start_pos_y = -10;
  float end_pos_y = 10;

  for (int i = 0; i < num_grid_lines; ++i) {
    pos_x = start_pos_x + i * (end_pos_x - start_pos_x) / num_grid_lines;
    pos_y = start_pos_y + i * (end_pos_y - start_pos_y) / num_grid_lines;
    grid.push_back(pos_x);
    grid.push_back(pos_y);
  }

  // Start the actual game loop of the application to show, render and interact
  // with the window.
  while (window.isOpen()) {
    // Get new mouse position and compute movement in space.
    const auto mouse_pos = sf::Mouse::getPosition(window);
    const int mouse_x = mouse_pos.x;
    const int mouse_y = mouse_pos.y;
    const me::vector2f mouse_move{
        view_dim.x / screen_width * (mouse_x - old_mouse_x),
        view_dim.y / screen_height * (mouse_y - old_mouse_y)};

    // Poll every event from the system and user.
    // Otherwise the window will not be displayed.
    sf::Event event{};
    while (window.pollEvent(event)) {
      // Decide what to do if certain events are happening.
      switch (event.type) {
        case sf::Event::Closed:
          window.close();
          break;

        case sf::Event::Resized:
          resize(event.size.width, event.size.height);
          update = true;
          break;

        case sf::Event::MouseWheelMoved:
          view_dim.y *= exp(-event.mouseWheel.delta * 0.05f);
          view_dim.y = clamp(view_dim.y, 1e-6f, 6.f);
          update = true;
          break;

        case sf::Event::KeyPressed:
          switch (event.key.code) {
            case sf::Keyboard::Escape:
              window.close();
              break;
            case sf::Keyboard::Left:
              pos_x -= speed;
              break;
            case sf::Keyboard::Right:
              pos_x += speed;
              break;
              // cout << pos_x << "\n";
            case sf::Keyboard::Down:
              pos_y += speed;
              break;
            case sf::Keyboard::Up:
              pos_y -= speed;
              break;
          }
          update = true;
          break;
      }
    }

    // Compute and render new view if needed.
    // The boolean variable 'update' makes sure this happens at most once per
    // loop iteration. Set it to true trigger the rendering process.
    if (update) {
      compute_viewport();
      update = false;
    }

    window.clear();

    // const me::vector2f x{0, 0};
    for (int i = 0; i < grid.size(); ++i) {
      const float p_x =
          (grid[2 * i] - view_min.x) / (view_max.x - view_min.x) * screen_width;
      const float p_y = (grid[2 * i + 1] - view_min.y) /
                        (view_max.y - view_min.y) * screen_height;
      ++i;
      // Move origin with left mouse button.
      if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        origin = origin - mouse_move;
        update = true;
      }

      sf::Vertex line_x[] = {sf::Vertex(sf::Vector2f(0, p_y)),
                             sf::Vertex(sf::Vector2f(500, p_y))};

      sf::Vertex line_y[] = {sf::Vertex(sf::Vector2f(p_x, 0)),
                             sf::Vertex(sf::Vector2f(p_x, 500))};

      window.draw(line_x, 2, sf::Lines);
      window.draw(line_y, 2, sf::Lines);
    }
    // }
    // Double Buffering
    window.display();

    old_mouse_x = mouse_x;
    old_mouse_y = mouse_y;
  }
}
