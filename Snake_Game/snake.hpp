#ifndef SNAKE_H
#define SNAKE_H

#include <SFML/Graphics.hpp>
#include <iostream>

class Snake 
{
private:
  int cols, lines, size, width, height, direction, snake_size, points;
  sf::RenderWindow window;
  sf::Texture background_t, fruit_t, snake_t;
  sf::Sprite background_spr, fruit_spr, snake_spr;
  sf::Clock clock;
  sf::Font font;
  sf::Text txt;
  float timer, delay;

  void verifyFiles();
  void collision();
  void move();
  void generateFruit();

public:
  Snake();
  void runGame(); 
};

#endif
