#include "ship.hpp"

Ship::Ship(std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<sf::Sprite> sprite, std::shared_ptr<sf::Sprite> bullet_spr)
{
  // pegando a janela
  this->window = window;
  // inicializando nave
  sprite = std::make_shared<sf::Sprite>();
  this->ship_spr = sprite;
  //  shoots
  bullet_spr = std::make_shared<sf::Sprite>();
  this->bullet_spr = bullet_spr;
  shoots = 20;
}

void Ship::moveShip()
{
  // movimento
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    ship_spr->move(15.f, 0.f);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    ship_spr->move(-15.f, 0.f);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    ship_spr->move(0.f, -15.f);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    ship_spr->move(0.f, 15.f);

  // colisao com as extremidades
  if (ship_spr->getPosition().x < 0)
    ship_spr->setPosition(0, ship_spr->getPosition().y);
  else if (ship_spr->getPosition().x > this->window->getSize().x - ship_spr->getGlobalBounds().width)
    ship_spr->setPosition(this->window->getSize().x - ship_spr->getGlobalBounds().width, ship_spr->getPosition().y);

  // colisao com as extremidades
  if (ship_spr->getPosition().y < ship_spr->getGlobalBounds().height)
  {
    ship_spr->setPosition(ship_spr->getPosition().x, ship_spr->getGlobalBounds().height);
  }
  else if (ship_spr->getPosition().y > this->window->getSize().y)
    ship_spr->setPosition(ship_spr->getPosition().x, this->window->getSize().y);
}

void Ship::shoot()
{
  {
    if (shoots < 12)
      ++shoots;

    // quando pressionar espaco adiciona um bullet
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && shoots >= 12)
    {
      bullet_spr->setPosition(sf::Vector2f(ship_spr->getPosition().x + static_cast<float>(ship_spr->getGlobalBounds().width) / 2 - 5.f,
                                           ship_spr->getPosition().y - static_cast<float>(ship_spr->getGlobalBounds().height) / 2 + 2.f));

      // vou adicionar a bullet que atirei no meu vetor
      bullets.push_back(*bullet_spr);
      shoots = 0;
    }

    // mover e deletar caso ultrapasse a janela
    for (size_t i{}; i < bullets.size(); ++i)
    {
      bullets[i].move(0, -20.f);
      if (bullets[i].getPosition().y < 0)
        bullets.erase(bullets.begin() + i);

      /*
            // colisao com a nave
            for (size_t k{}; k < enemies.size(); ++k)
            {
              if (bullets[i].getGlobalBounds().intersects(enemies[k].getGlobalBounds()))
              {
                show = false;
                max = 0;
                score += 10;
                explosion_spr->setPosition(enemies[k].getPosition().x, enemies[k].getPosition().y);
                bullets.erase(bullets.begin() + i);
                enemies.erase(enemies.begin() + k);
              }
            }
            */
    }
  }
}
