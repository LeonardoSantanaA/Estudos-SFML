#include "minecrap.hpp"

int main()
{
  // na classe  Minecrap eu aloquei todo mundo na stack, porem vou rodar o game na heap, por isso vou usar ponteiros:
  Minecrap *minecrap = new Minecrap();
  minecrap->runGame();
  delete minecrap;
  minecrap = nullptr;
  return EXIT_SUCCESS;
}
