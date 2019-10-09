#include <application.hpp>
#include <random>

#include <cmath>
#include <iostream>

void application::execute() {
  using namespace std;

  constexpr int num_grid_lines = 30;

  float center_pos_x = 0;
  float center_pos_y = 0;

  float size_circle = 20;

  vector<float> objectives(20);

  random_device rd{};
  mt19937 rng{rd()};
  uniform_real_distribution<float> dist{-1, 1};

  for (int i = 0; i < 60; ++i) {
    float randn = dist(rng);
    if (randn <= 0) {
      objectives.push_back(dist(rng) - 1);
    } else {
      objectives.push_back(dist(rng) + 1);
    }
  }

  int old_mouse_x = 0;
  int old_mouse_y = 0;

  std::vector<float> grid;
  float start_pos_x = -10;
  float end_pos_x = 10;
  float start_pos_y = -10;
  float end_pos_y = 10;

  float pos_x;
  float pos_y;

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
        }
        update = true;
        break;
      }
    }

    // Move origin with left mouse button.
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      origin = origin - mouse_move;
      update = true;
    }
    // Compute and render new view if needed.
    // The boolean variable 'update' makes sure this happens at most once per
    // loop iteration. Set it to true trigger the rendering process.
    if (update) {
      compute_viewport();
      update = false;
    }

    window.clear();

    float velocity = 100;

    float accel_x = mouse_x - screen_width / 2;
    float accel_y = mouse_y - screen_height / 2;

    if (abs(accel_x) > velocity) {
      accel_x = velocity * accel_x / abs(accel_x);
    }
    if (abs(accel_y) > velocity) {
      accel_y = velocity * accel_y / abs(accel_y);
    }

    if (-abs(center_pos_x) < start_pos_x) {
      accel_x = -accel_x * 1000;
    }

    if (-abs(center_pos_y) < start_pos_y) {
      accel_y = -accel_y * 1000;
    }

    // const me::vector2f x{0, 0};
    for (int i = 0; i < grid.size(); i = i + 2) {
      grid[i] -= 1e-5f * accel_x;
      grid[i + 1] -= 1e-5f * accel_y;

      const float p_x =
          (grid[i] - view_min.x) / (view_max.x - view_min.x) * screen_width;
      const float p_y = (grid[i + 1] - view_min.y) / (view_max.y - view_min.y) *
                        screen_height;

      sf::Vertex line_x[] = {sf::Vertex(sf::Vector2f(0, p_y)),
                             sf::Vertex(sf::Vector2f(screen_width, p_y))};

      sf::Vertex line_y[] = {sf::Vertex(sf::Vector2f(p_x, 0)),
                             sf::Vertex(sf::Vector2f(p_x, screen_height))};

      window.draw(line_x, 2, sf::Lines);
      window.draw(line_y, 2, sf::Lines);
    }
    // }

    for (int i = 0; i < objectives.size(); i = i + 2) {
      objectives[i] -= 1e-5f * accel_x;
      objectives[i + 1] -= 1e-5f * accel_y;

      // cout << "Objectives left: " << objectives.size() / 2 << "\n";

      if (sqrt(pow(objectives[i], 2) + pow(objectives[i + 1], 2)) < 0.1) {
        std::swap(objectives[i], objectives[objectives.size() - 2]);
        std::swap(objectives[i + 1], objectives[objectives.size() - 1]);
        objectives.resize(objectives.size() - 2);
      }

      const float p_goal_x = (objectives[i] - view_min.x) /
                             (view_max.x - view_min.x) * screen_width;
      const float p_goal_y = (objectives[i + 1] - view_min.y) /
                             (view_max.y - view_min.y) * screen_height;

      sf::CircleShape circle_goal(5);
      circle_goal.setPosition(p_goal_x, p_goal_y);
      circle_goal.setOrigin(5, 5);
      window.draw(circle_goal);
    }

    center_pos_x -= 1e-5f * accel_x;
    center_pos_y -= 1e-5f * accel_y;

    cout << "Pos in Grid: " << center_pos_x << ", " << center_pos_y << "\n";

    const float center_x =
        (0 - view_min.x) / (view_max.x - view_min.x) * screen_width;
    const float center_y =
        (0 - view_min.y) / (view_max.y - view_min.y) * screen_height;

    sf::CircleShape circle(size_circle);
    circle.setPosition(center_x, center_y);
    circle.setOrigin(size_circle, size_circle);
    window.draw(circle);

    // Double Buffering
    window.display();

    old_mouse_x = mouse_x;
    old_mouse_y = mouse_y;
  }
}
