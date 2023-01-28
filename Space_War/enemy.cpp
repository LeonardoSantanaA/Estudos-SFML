#include "enemy.hpp"

Enemy::Enemy(std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<sf::Sprite> sprite)
{
  this->window = window;
  this->enemy_spr = sprite;
  spawn_enemies = 0;
}

void Enemy::spawnEnemy()
{
  if (spawn_enemies < 20)
    ++spawn_enemies;

  // gerando inimigos aleatoriamente
  if (spawn_enemies >= 20)
  {

    enemy_spr->setPosition(
        std::rand() % window->getSize().x - enemy_spr->getGlobalBounds().width,
        0);
    // vou adicionar a inimigo gerado no vetor
    enemies.push_back(*enemy_spr);
    spawn_enemies = 0;
  }

  for (size_t i{}; i < enemies.size(); ++i)
  {
    enemies[i].move(0, 10.f);
    // garantia que nenhum vai nascer fora da tela
    if (enemies[i].getPosition().x + enemy_spr->getGlobalBounds().width + 20.f > window->getSize().x ||
        enemies[i].getPosition().x + enemy_spr->getGlobalBounds().width < 0)
    {
      if (enemies[i].getPosition().y > window->getSize().y)
        enemies.erase(enemies.begin() + i);
    }
  }
}
