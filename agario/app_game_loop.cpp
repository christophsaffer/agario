#include <application.hpp>

#include <cmath>
#include <iostream>

void application::execute() {
  using namespace std;

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
      update = false;
    }

    window.clear();

    // Double Buffering
    window.display();

    old_mouse_x = mouse_x;
    old_mouse_y = mouse_y;
  }
}
