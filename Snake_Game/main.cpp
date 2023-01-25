#include "snake.hpp"
#include <cstdlib>

int main()
{
  Snake *snake = new Snake();
  snake->runGame();
  delete snake;
  snake = nullptr;

  return EXIT_SUCCESS;
}
