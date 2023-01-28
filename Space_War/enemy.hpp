#ifndef ENEMY_H_
#define ENEMY_H_

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <vector>

class Enemy
{
private:
  int spawn_enemies = 0;
  std::shared_ptr<sf::RenderWindow> window;

public:
  std::shared_ptr<sf::Sprite> enemy_spr;
  std::vector<sf::Sprite> enemies;

  Enemy(std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<sf::Sprite> sprite);
  void spawnEnemy();
};

#endif