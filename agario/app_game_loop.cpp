#include <application.hpp>
#include <random>

#include <cmath>
#include <iostream>

void application::execute() {
  using namespace std;

  float pos_x = 0;
  float pos_y = 0;

  float pos_goal_x = 1;
  float pos_goal_y = -1;

  float size_circle = 5;

  vector<float> objectives(20);

  random_device rd{};
  mt19937 rng{rd()};
  uniform_real_distribution<float> dist{-2, 2};

  for (int i = 0; i < 20; ++i) {
    objectives.push_back(dist(rng));
  }

  int old_mouse_x = 0;
  int old_mouse_y = 0;

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
      //
      // pos_goal_x -= 1e-3f * (old_mouse_x - mouse_x);
      // pos_goal_y -= 1e-3f * (old_mouse_y - mouse_y);

      update = false;
    }

    window.clear();

    for (int i = 0; i < objectives.size(); i = i + 2) {

      objectives[i] -= 1e-5f * (mouse_x - screen_width / 2);
      objectives[i + 1] -= 1e-5f * (mouse_y - screen_height / 2);

      if (sqrt(pow(objectives[i], 2) + pow(objectives[i + 1], 2)) <
          (size_circle * 1e-2f)) {
        std::swap(objectives[i], objectives[objectives.size() - 2]);
        std::swap(objectives[i + 1], objectives[objectives.size() - 1]);
        objectives.resize(objectives.size() - 2);
        size_circle += 2;
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

    const float p_x =
        (pos_x - view_min.x) / (view_max.x - view_min.x) * screen_width;
    const float p_y =
        (pos_y - view_min.y) / (view_max.y - view_min.y) * screen_height;

    sf::CircleShape circle(size_circle);
    circle.setPosition(p_x, p_y);
    circle.setOrigin(size_circle, size_circle);
    window.draw(circle);

    // Double Buffering
    window.display();

    old_mouse_x = mouse_x;
    old_mouse_y = mouse_y;
  }
}
