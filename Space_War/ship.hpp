#ifndef SHIP_H_
#define SHIP_H_

#include <iostream>
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>

class Ship
{
public:
  std::shared_ptr<sf::RenderWindow> window;
  std::shared_ptr<sf::Sprite> ship_spr;
  std::shared_ptr<sf::Sprite> bullet_spr;
  std::shared_ptr<sf::Texture> ship_t;
  int shoots;
  std::vector<sf::Sprite> bullets;

  Ship(std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<sf::Sprite> sprite, std::shared_ptr<sf::Sprite> bullet_spr);
  void moveShip();
  void shoot();
};

#endif