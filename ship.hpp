#ifndef SHIP_H_
#define SHIP_H_

#include <iostream>
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>

class Ship
{
private:
  std::shared_ptr<sf::RenderWindow> window;
  int shoots;

public:
  std::shared_ptr<sf::Sprite> ship_spr;
  std::shared_ptr<sf::Sprite> bullet_spr;
  std::vector<sf::Sprite> bullets;

  Ship(std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<sf::Sprite> sprite, std::shared_ptr<sf::Sprite> bullet_spr);
  void moveShip();
  void shoot();
};

#endif