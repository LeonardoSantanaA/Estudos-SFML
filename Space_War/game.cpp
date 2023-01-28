#include "game.hpp"

// Construtor
Game::Game()
{
  window = std::make_shared<sf::RenderWindow>(sf::VideoMode(1280, 720), "Space War 0.4", sf::Style::Titlebar | sf::Style::Close);
  window->setFramerateLimit(60);
  window->setPosition(sf::Vector2i(30, 30));
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
  // iniciando texts
  score_txt = std::make_shared<sf::Text>();
  score_txt->setFont(*font);
  gameover_txt = std::make_shared<sf::Text>();
  gameover_txt->setFont(*font);
  paused_txt = std::make_shared<sf::Text>();
  paused_txt->setFont(*font);
  info_txt = std::make_shared<sf::Text>();
  info_txt->setFont(*font);
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
}

void Game::startGame()
{
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

    // sistemas do jogo
    ship->moveShip();
    ship->shoot();

    window->clear();
    window->draw(*background_spr);
    for (auto e : ship->bullets)
      window->draw(e);

    window->draw(*ship->ship_spr);

    window->display();
  }
}