#include <vector>

/*
Idea:
- Class contains all information about the states of the game namely:
  - grid and size of the field
  - number, states and real position of all players
  - number, size and position of all fruits
- the calculation of the pixel values happens in the class application of which
  each (realworld) player creates a unique instance when starting the game
- execution of functions application::execute yields either
  a new game instance (new game) or a reference to an existing game (join game)
- the state of the game gets updated with every loop in "app_game_loop"



*/
class game {
 public:
  game() {
    generate_grid();
    generate_fruits();
    add_player()
  };

 public:
  void generate_grid();
  void generate_fruits();
  void add_player(){};
  void update_game_state(){};

 public:
  std::vector<player> list_players;
  std::vector<float> grid;

  std::vector<float> fruits;
};