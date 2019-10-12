#include <application.hpp>
#include <array>
#include <random>

#include <cmath>
#include <iostream>

void application::execute() {
  using namespace std;

  float pos_x = 0;
  float pos_y = 0;

  float size_circle = 0.05;
  float size_obj = 0.01;

  vector<float> objectives(20);

  random_device rd{};
  mt19937 rng{rd()};
  uniform_real_distribution<float> dist{-2, 2};

  for (int i = 0; i < 20; ++i) {
    objectives.push_back(dist(rng));
  }

  int old_mouse_x = 0;
  int old_mouse_y = 0;

  constexpr int num_grid_lines = 30;
  std::vector<float> grid;
  float start_pos_x = -10;
  float end_pos_x = 10;
  float start_pos_y = -10;
  float end_pos_y = 10;

  for (int i = 0; i < num_grid_lines + 1; ++i) {
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

    float velocity = 200;

    float accel_x = mouse_x - screen_width / 2;
    float accel_y = mouse_y - screen_height / 2;

    if (abs(accel_x) > velocity) {
      accel_x = velocity * accel_x / abs(accel_x);
    }
    if (abs(accel_y) > velocity) {
      accel_y = velocity * accel_y / abs(accel_y);
    }

    pos_x += 1e-5f * accel_x;
    pos_y += 1e-5f * accel_y;

    // cout << pos_x << ", " << pos_y << "\n";

    // const me::vector2f x{0, 0};
    for (int i = 0; i < grid.size(); i = i + 2) {
      // grid[i] -= 1e-5f * accel_x;
      // grid[i + 1] -= 1e-5f * accel_y;

      // const float p_x =
      //     (grid[i] - view_min.x) / (view_max.x - view_min.x) * screen_width;
      // const float p_y = (grid[i + 1] - view_min.y) / (view_max.y -
      // view_min.y) *
      //                   screen_height;

      // array<float, 2> pos_line = compute_pixel(grid[i], grid[i + 1]);

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
      //
      // sf::Vertex line_x[] = {
      //     sf::Vertex(sf::Vector2f(0, pos_line[1])),
      //     sf::Vertex(sf::Vector2f(screen_width, pos_line[1]))};
      //
      // sf::Vertex line_y[] = {sf::Vertex(sf::Vector2f(pos_line[0], 0)),
      //                        sf::Vertex(sf::Vector2f(pos_line[0],
      //                        screen_height))};

      window.draw(line_x, 2, sf::Lines);
      window.draw(line_y, 2, sf::Lines);
    }
    // }

    for (int i = 0; i < objectives.size(); i = i + 2) {
      // objectives[i] -= 1e-5f * accel_x;
      // objectives[i + 1] -= 1e-5f * accel_y;

      // cout << "Objectives left: " << objectives.size() / 2 << "\n";

      // if (sqrt(pow(objectives[i], 2) + pow(objectives[i + 1], 2)) < 0.1) {
      if (sqrt(pow(objectives[i] - origin.x, 2) +
               pow(objectives[i + 1] - origin.y, 2)) < size_obj + size_circle) {
        std::swap(objectives[i], objectives[objectives.size() - 2]);
        std::swap(objectives[i + 1], objectives[objectives.size() - 1]);
        objectives.resize(objectives.size() - 2);
        size_circle += 0.01;
        velocity -= 10;
      }

      // const float p_goal_x = (objectives[i] - view_min.x) /
      //                        (view_max.x - view_min.x) * screen_width;
      // const float p_goal_y = (objectives[i + 1] - view_min.y) /
      //                        (view_max.y - view_min.y) * screen_height;

      array<float, 2> pos_objective =
          compute_pixel(objectives[i], objectives[i + 1]);

      float radius_obj = scale_length(size_obj);

      sf::CircleShape circle_objective(radius_obj);
      circle_objective.setPosition(pos_objective[0], pos_objective[1]);
      circle_objective.setOrigin(radius_obj, radius_obj);
      window.draw(circle_objective);
    }

    origin.x += 1e-5f * accel_x;
    origin.y += 1e-5f * accel_y;

    compute_viewport();

    array<float, 2> center = compute_pixel(origin.x, origin.y);

    cout << origin.x << ", " << origin.y << "\n";

    float radius = scale_length(size_circle);

    sf::CircleShape circle(radius);
    circle.setPosition(center[0], center[1]);
    circle.setOrigin(radius, radius);
    window.draw(circle);

    // Double Buffering
    window.display();

    old_mouse_x = mouse_x;
    old_mouse_y = mouse_y;
  }
}
