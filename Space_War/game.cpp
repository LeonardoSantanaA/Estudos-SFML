#include "game.hpp"

// Construtor
Game::Game()
{
  window = std::make_shared<sf::RenderWindow>(sf::VideoMode(1280, 720), "Space War 0.5", sf::Style::Titlebar | sf::Style::Close);
  window->setFramerateLimit(60);
  window->setPosition(sf::Vector2i(30, 30));
  spritesheet_y = 0;
  score = 0;
  gameover = false;
  paused = false;
  frame = 0.f, speedFrame = 0.4f;
  count = 13, max = 0; // quantidade de frames, max é para limitar a quantidade de frames
  show = true;         // mostrar a animacao apenas uma vez
  // iniciando texturas
  ship_t = std::make_shared<sf::Texture>();
  bullet_t = std::make_shared<sf::Texture>();
  enemy_t = std::make_shared<sf::Texture>();
  life_t = std::make_shared<sf::Texture>();
  background_t = std::make_shared<sf::Texture>();
  explosion_t = std::make_shared<sf::Texture>();
  // iniciando font
  font = std::make_shared<sf::Font>();
  // carregar os arquivos
  try
  {
    this->loadFiles();
  }
  catch (const char *msg)
  {
    std::cerr << msg << '\n';
    window->close();
  }
  // iniciando sprites
  ship_spr = std::make_shared<sf::Sprite>(*ship_t);
  enemy_spr = std::make_shared<sf::Sprite>(*enemy_t);
  bullet_spr = std::make_shared<sf::Sprite>(*bullet_t);
  spritesheet_life = std::make_shared<sf::Sprite>(*life_t);
  background_spr = std::make_shared<sf::Sprite>(*background_t);
  explosion_spr = std::make_shared<sf::Sprite>(*explosion_t);
  explosion_spr->setTextureRect(sf::IntRect(0, 0, 80, 97));
  spritesheet_life->setPosition(
      static_cast<float>(window->getSize().x) / 2 - static_cast<float>(life_t->getSize().x) / 2, 690.f);
  // iniciando texts
  score_txt = std::make_shared<sf::Text>();
  score_txt->setFont(*font);
  score_txt->setFillColor(sf::Color::White);
  score_txt->setPosition(30, 30);
  gameover_txt = std::make_shared<sf::Text>();
  gameover_txt->setFont(*font);
  gameover_txt->setFillColor(sf::Color::White);
  gameover_txt->setCharacterSize(150);
  gameover_txt->setString("GAME OVER");
  gameover_txt->setPosition((float)(window->getSize().x) / 2 - 225.f, (float)(window->getSize().y) / 2 - 150.f);
  paused_txt = std::make_shared<sf::Text>();
  paused_txt->setFont(*font);
  paused_txt->setFillColor(sf::Color::White);
  paused_txt->setCharacterSize(150);
  paused_txt->setString("PAUSED");
  paused_txt->setPosition((float)(window->getSize().x) / 2 - 135.f, (float)(window->getSize().y) / 2 - 150.f);
  info_txt = std::make_shared<sf::Text>();
  info_txt->setFont(*font);
  info_txt->setFillColor(sf::Color::Yellow);
  info_txt->setCharacterSize(70);
  // iniciando objetos
  // ship
  ship = std::make_shared<Ship>(window, ship_spr, bullet_spr);
  ship->ship_spr = ship_spr;     // foi a solucao que encontrei pra fazer a nave aparecer
  ship->bullet_spr = bullet_spr; // foi a solucao que encontrei pra fazer a bullet aparecer
  ship_spr->setRotation(270.f);  // rotaciono a nave pra deixar ela virada pra cima
  // eu pego o tamanho do y da minha nave pois ela originalmente é virada pra horizontal
  ship_spr->setPosition(static_cast<float>(window->getSize().x) / 2 - static_cast<float>(ship_t->getSize().y) / 2, 500.f);

  // bullet
  bullet_spr->setRotation(270.f);
  bullet_spr->setScale(0.1f, 0.1f);

  // enemy
  enemy = std::make_shared<Enemy>(window, enemy_spr);
  enemy->enemy_spr = enemy_spr;
}

void Game::loadFiles()
{
  if (!ship_t->loadFromFile("./assets/img/spaceship-min.png"))
    throw "Não foi possível carregar a imagem da nave principal! Verifique a integridade dos seus arquivos.";
  if (!bullet_t->loadFromFile("./assets/img/bullet-original.png"))
    throw "Não foi possível carregar a imagem dos bullets! Verifique a integridade dos seus arquivos.";
  if (!enemy_t->loadFromFile("./assets/img/enemy-min.png"))
    throw "Não foi possivel carregar a imagem dos inimigos! Verifique a integridade dos seus arquivos.";
  if (!life_t->loadFromFile("./assets/img/power-life.png"))
    throw "Não foi possível carregar a imagem da vida! Verifique a integridade dos seus arquivos";
  if (!background_t->loadFromFile("./assets/img/bg.jpg"))
    throw "Não foi possível carregar a imagem do background! Verifique a integridade dos seus arquivos.";
  if (!font->loadFromFile("./assets/font/Ribheud.ttf"))
    throw "Não foi possível carregar a font! Verifique a integridade dos seus arquivos.";
  if (!explosion_t->loadFromFile("./assets/img/explosion.png"))
    throw "Não foi possível carregar a spritesheet de explosão! Verifique a integridade dos seus arquivos.";
  if (!music.openFromFile("./assets/snd/SpaceWar_OST(Pedro A.).wav"))
    throw "Não foi possível carregar a musica de fundo! Verifique a integridade dos seus arquivos.";
}

void Game::animationExplosion()
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

void Game::collision()
{
  // bullet
  // mover e deletar caso ultrapasse a janela
  for (size_t i{}; i < ship->bullets.size(); ++i)
  {

    // colisao com a nave
    for (size_t k{}; k < enemy->enemies.size(); ++k)
    {
      if (ship->bullets[i].getGlobalBounds().intersects(enemy->enemies[k].getGlobalBounds()))
      {
        show = false;
        max = 0;
        score += 10;
        explosion_spr->setPosition(enemy->enemies[k].getPosition().x, enemy->enemies[k].getPosition().y);
        ship->bullets.erase(ship->bullets.begin() + i);
        enemy->enemies.erase(enemy->enemies.begin() + k);
      }
    }
  }

  // enemy
  //  colisao com a nave
  for (size_t i{}; i < enemy->enemies.size(); ++i)
  {
    if (enemy->enemies[i].getGlobalBounds().intersects(ship_spr->getGlobalBounds()))
    {
      enemy->enemies.erase(enemy->enemies.begin() + i);
      show = false;
      max = 0;
      score += 10;
      explosion_spr->setPosition(ship_spr->getPosition().x + static_cast<float>(ship_spr->getGlobalBounds().width) / 2 - 30.f,
                                 ship_spr->getPosition().y - static_cast<float>(ship_spr->getGlobalBounds().height) / 2 - 45.f);
      spritesheet_y += 30;
      if (spritesheet_y < 120)
        spritesheet_life->setTextureRect(sf::IntRect(0, spritesheet_y, 300, 30));
      else
        gameover = true;
    }
  }
}

void Game::flowGame()
{
  // sistemas do jogo
  if (music.getStatus() == sf::Music::Stopped || music.getStatus() == sf::Music::Paused)
  {
    if (!paused) // resolve um pequeno bug sonoro
      music.play();
  }

  ship->moveShip();
  ship->shoot();
  enemy->spawnEnemy();
  animationExplosion();
  collision();

  window->clear();
  window->draw(*background_spr);
  window->draw(*ship->ship_spr);
  for (auto b : ship->bullets)
    window->draw(b);

  for (auto e : enemy->enemies)
    window->draw(e);
  if (max <= 1040.f && !show)
    window->draw(*explosion_spr);
  score_txt->setString("Score: " + std::to_string(score));
  window->draw(*score_txt);

  window->draw(*spritesheet_life);

  window->display();
}

void Game::gameOver()
{
  music.stop();
  info_txt->setPosition((float)(window->getSize().x) / 2 - 300.f, (float)(window->getSize().y) / 2);
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

void Game::pause()
{
  music.pause();
  info_txt->setPosition((float)(window->getSize().x) / 2 - 300.f, (float)(window->getSize().y) / 2);
  info_txt->setString("Pressione ENTER para continuar");
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
    paused = false;
  window->draw(*paused_txt);
  window->draw(*info_txt);
  window->display();
}

void Game::startGame()
{
  while (window->isOpen())
  {
    sf::Event event;

    while (window->pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window->close();
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        paused = true;
    }
    if (gameover)
      gameOver();
    else
    {
      if (paused)
        pause();
      else
        flowGame();
    }
  }
}
