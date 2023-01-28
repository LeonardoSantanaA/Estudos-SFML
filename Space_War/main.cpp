#include "game.hpp"

int main()
{
  Game *game = new Game();
  game->startGame();
  delete game;
  game = nullptr;
}

// TODO adicionar score
// TODO sonosplastia
/*

void bullet(std::shared_ptr<sf::Sprite> bullet, std::shared_ptr<sf::Sprite> spaceship, std::shared_ptr<sf::Texture> bullet_t,
            std::shared_ptr<sf::Texture> spaceship_t, std::vector<sf::Sprite> &bullets, int &shoot, std::vector<sf::Sprite> &enemies,
            float &frame, float &speedFrame, std::shared_ptr<sf::Sprite> explosion_spr, float &max, float &count, bool &show, int &score)
{
  if (shoot < 12)
    ++shoot;

  // quando pressionar espaco adiciona um bullet
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && shoot >= 12)
  {
    bullet->setPosition(sf::Vector2f(spaceship->getPosition().x + static_cast<float>(spaceship_t->getSize().y) / 2 - 5.f,
                                     spaceship->getPosition().y - static_cast<float>(spaceship_t->getSize().x) / 2 + 2.f));

    // vou adicionar a bullet que atirei no meu vetor
    bullets.push_back(*bullet);
    shoot = 0;
  }

  // mover e deletar caso ultrapasse a janela
  for (size_t i{}; i < bullets.size(); ++i)
  {
    bullets[i].move(0, -20.f);
    if (bullets[i].getPosition().y < 0)
      bullets.erase(bullets.begin() + i);

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
  }
}

void enemiesSystem(int &spawn_enemies, std::shared_ptr<sf::Sprite> enemy_spr, std::shared_ptr<sf::RenderWindow> window,
                   std::vector<sf::Sprite> &enemies, std::shared_ptr<sf::Sprite> spaceship, std::shared_ptr<sf::Sprite> spritesheet_life, int &spritesheet_y, bool &gameover)
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
    // colisao com a nave
    if (enemies[i].getGlobalBounds().intersects(spaceship->getGlobalBounds()))
    {
      enemies.erase(enemies.begin() + i);
      spaceship->setColor(sf::Color::Red);
      spritesheet_y += 30;
      if (spritesheet_y < 120)
        spritesheet_life->setTextureRect(sf::IntRect(0, spritesheet_y, 300, 30));
      else
        gameover = true;
    }
  }
}

void animationExplosion(float &max, float &frame, float count, float speedFrame, bool show, std::shared_ptr<sf::Sprite> explosion_spr)
{
  // responsável pela imagem não ir tão rapido
  if (max <= 1040.f && !show)
  {
    frame += speedFrame;
    if (frame > count)
      frame = 0.f;

    explosion_spr->setTextureRect(sf::IntRect((int)frame * 80, 0, 80, 97));
  }
  max = max + 80;
}

int main()
{
  std::srand(std::time(nullptr));



  // bullet

  // eu nao vou criar o vetor como um ponteiro inteligente, isso por causa de um delay que existe nesses arrays, ja que é armazenado em forma de pilha
  std::vector<sf::Sprite> bullets, enemies;
  // limita os tiros e inimigos
  int shoot = 20, spawn_enemies = 1;

  // enemies

  // vamos setar a posicao do inimigo de forma randomica
  enemy_spr->setPosition(
      std::rand() % window->getSize().x + enemy_spr->getGlobalBounds().width,
      0);
  enemies.push_back(*enemy_spr);

  // life

  spritesheet_life->setPosition(
      static_cast<float>(window->getSize().x) / 2 - static_cast<float>(life_t->getSize().x) / 2, 690.f);

  // nós precisamos apenas de uma parte dessa spritesheet, para isso, vamos fazer:
  spritesheet_life->setTextureRect(sf::IntRect(0, 0, 300, 30));
  int spritesheet_y = 0; // controla a posicao na spritesheet

  // score
  int score;

  score_txt->setFillColor(sf::Color::White);
  score_txt->setPosition(30, 30);

  // background

  // gameover
  bool gameover = false;

  gameover_txt->setFillColor(sf::Color::White);
  gameover_txt->setCharacterSize(150);
  gameover_txt->setString("GAME OVER");
  gameover_txt->setPosition(
      (float)(window->getSize().x) / 2 - 225.f, (float)(window->getSize().y) / 2 - 150.f);

  // pause
  bool paused = false;

  paused_txt->setFillColor(sf::Color::White);
  paused_txt->setCharacterSize(150);
  paused_txt->setString("PAUSED");
  paused_txt->setPosition(
      (float)(window->getSize().x) / 2 - 135.f, (float)(window->getSize().y) / 2 - 150.f);

  // information text

  info_txt->setFillColor(sf::Color::Yellow);
  info_txt->setCharacterSize(70);

  // explosion spritesheet

  explosion_spr->setTextureRect(sf::IntRect(0, 0, 80, 97));
  float frame = 0.f, speedFrame = 0.4f;
  float count = 13, max = 0; // quantidade de frames, max é para limitar a quantidade de frames
  bool show = true;          // mostrar a animacao apenas uma vez

  while (window->isOpen())
  {
    sf::Event event;
    while (window->pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        window->close();
      }
    }
    if (gameover)
    {
      info_txt->setPosition(
          (float)(window->getSize().x) / 2 - 300.f, (float)(window->getSize().y) / 2);
      info_txt->setString("Pressione ENTER para reiniciar");
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
      {
        score = 0;
        spritesheet_y = 0;
        spritesheet_life->setTextureRect(sf::IntRect(0, spritesheet_y, 300, 30));
        gameover = false;
      }

      window->clear();
      window->draw(*gameover_txt);
      window->draw(*info_txt);
      window->display();
    }
    else
    {
      if (paused)
      {
        info_txt->setPosition(
            (float)(window->getSize().x) / 2 - 300.f, (float)(window->getSize().y) / 2);
        info_txt->setString("Pressione ENTER para continuar");

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
          paused = false;

        window->draw(*paused_txt);
        window->draw(*info_txt);
        window->display();
      }
      else
      {
        // sistemas do jogo
        // mover e deletar caso ultrapasse a janela

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
          paused = true;

        controlShip(spaceship_spr, spaceship_t, window);
        bullet(bullet_spr, spaceship_spr, bullet_t, spaceship_t, bullets, shoot, enemies, frame, speedFrame, explosion_spr, max, count, show, score);
        enemiesSystem(spawn_enemies, enemy_spr, window, enemies, spaceship_spr, spritesheet_life, spritesheet_y, gameover);
        animationExplosion(max, frame, count, speedFrame, show, explosion_spr);

        window->clear();
        window->draw(*background_spr);
        window->draw(*spaceship_spr);
        // desenho das bullets
        for (auto &b : bullets)
          window->draw(b);

        // desenho dos inimigos
        for (auto &e : enemies)
          window->draw(e);
        if (max <= 1040.f && !show)
          window->draw(*explosion_spr);

        window->draw(*spritesheet_life);

        score_txt->setString("Score: " + std::to_string(score));
        window->draw(*score_txt);
        window->display();
      }
    }
  }

  return EXIT_SUCCESS;
}
*/