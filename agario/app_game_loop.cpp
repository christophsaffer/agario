#include <application.hpp>
#include <player.hpp>

#include <array>
#include <cmath>
#include <iostream>
#include <random>

void application::execute() {
  using namespace std;

  // player player1();
  // float size_circle = 0.05;
  float size_obj = 0.01;

  vector<float> fruits(20);
  random_device rd{};
  mt19937 rng{rd()};
  uniform_real_distribution<float> dist{-2, 2};
  for (int i = 0; i < 20; ++i) {
    fruits.push_back(dist(rng));
  }

  int old_mouse_x = 0;
  int old_mouse_y = 0;

  // initialize grid
  constexpr int num_grid_lines = 30;
  std::vector<float> grid;
  float start_pos_x = -10;
  float end_pos_x = 10;
  float start_pos_y = -10;
  float end_pos_y = 10;

  for (int i = 0; i < num_grid_lines + 1; ++i) {
    grid.push_back(start_pos_x +
                   i * (end_pos_x - start_pos_x) / num_grid_lines);
    grid.push_back(start_pos_y +
                   i * (end_pos_y - start_pos_y) / num_grid_lines);
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
          break;

        case sf::Event::MouseWheelMoved:
          view_dim.y *= exp(-event.mouseWheel.delta * 0.05f);
          view_dim.y = clamp(view_dim.y, 1e-6f, 6.f);
          break;

        case sf::Event::KeyPressed:
          switch (event.key.code) {
            case sf::Keyboard::Escape:
              window.close();
              break;
          }
          break;
      }
    }
    // Move origin with left mouse button.
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      origin = origin - mouse_move;
    }

    window.clear();

    // compute velocity

    player1.compute_velocity(mouse_x, mouse_y, screen_width, screen_height);
    /* float velocity = 200;
     float veloc_x = mouse_x - screen_width / 2;
     float veloc_y = mouse_y - screen_height / 2;
     if (abs(veloc_x) > velocity) {
       veloc_x = velocity * veloc_x / abs(veloc_x);
     }
     if (abs(veloc_y) > velocity) {
       veloc_y = velocity * veloc_y / abs(veloc_y);
     }
 */
    // draw grid
    for (int i = 0; i < grid.size(); i = i + 2) {
      array<float, 2> start_hor_line = compute_pixel(start_pos_x, grid[i + 1]);
      array<float, 2> end_hor_line = compute_pixel(end_pos_x, grid[i + 1]);
      array<float, 2> start_vert_line = compute_pixel(grid[i], start_pos_y);
      array<float, 2> end_vert_line = compute_pixel(grid[i], end_pos_y);

      sf::Vertex line_x[] = {
          sf::Vertex(sf::Vector2f(start_vert_line[0], start_vert_line[1])),
          sf::Vertex(sf::Vector2f(end_vert_line[0], end_vert_line[1]))};

      sf::Vertex line_y[] = {
          sf::Vertex(sf::Vector2f(start_hor_line[0], start_hor_line[1])),
          sf::Vertex(sf::Vector2f(end_hor_line[0], end_hor_line[1]))};

      window.draw(line_x, 2, sf::Lines);
      window.draw(line_y, 2, sf::Lines);
    }
    // }

    for (int i = 0; i < fruits.size(); i = i + 2) {
      if (sqrt(pow(fruits[i] - origin.x, 2) +
               pow(fruits[i + 1] - origin.y, 2)) <
          size_obj + player1.circle_sizes[0]) {
        std::swap(fruits[i], fruits[fruits.size() - 2]);
        std::swap(fruits[i + 1], fruits[fruits.size() - 1]);
        fruits.resize(fruits.size() - 2);
        player1.circle_sizes[0] += 0.01;
        player1.max_velocity -= 10;
      }

      array<float, 2> pos_objective = compute_pixel(fruits[i], fruits[i + 1]);
      float radius_obj = scale_length(size_obj);

      sf::CircleShape circle_objective(radius_obj);
      circle_objective.setPosition(pos_objective[0], pos_objective[1]);
      circle_objective.setOrigin(radius_obj, radius_obj);
      window.draw(circle_objective);
    }

    origin.x += 1e-5f * player1.velocity[0];
    origin.y += 1e-5f * player1.velocity[1];
    compute_viewport();

    array<float, 2> center = compute_pixel(origin.x, origin.y);

    cout << origin.x << ", " << origin.y << "\n";

    // loop for multiple circles for example after split
    for (int i = 0; i < player1.circle_sizes.size(); ++i) {
      float radius = scale_length(player1.circle_sizes[i]);

      sf::CircleShape circle(radius);
      circle.setPosition(center[0], center[1]);
      circle.setOrigin(radius, radius);
      window.draw(circle);
    }

    // Double Buffering
    window.display();

    old_mouse_x = mouse_x;
    old_mouse_y = mouse_y;
  }
}
