#include "game.hpp"

int main()
{
  std::srand(std::time(nullptr));
  Game *game = new Game();

  game->startGame();
  delete game;
  game = nullptr;
}
