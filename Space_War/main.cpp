#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

// TODO adicionar score
// TODO sonosplastia

void controlShip(std::shared_ptr<sf::Sprite> spaceship, std::shared_ptr<sf::Texture> spaceship_t, std::shared_ptr<sf::RenderWindow> window)
{
  // movimento
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    spaceship->move(15.f, 0.f);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    spaceship->move(-15.f, 0.f);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    spaceship->move(0.f, -15.f);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    spaceship->move(0.f, 15.f);

  // colisao com as extremidades
  // estou usando o spaceship_t->getSize().y trocado com x por causa da nave rotacionada!
  if (spaceship->getPosition().x < 0)
    spaceship->setPosition(0, spaceship->getPosition().y);
  else if (spaceship->getPosition().x > window->getSize().x - spaceship_t->getSize().y)
    spaceship->setPosition(window->getSize().x - spaceship_t->getSize().y, spaceship->getPosition().y);
  if (spaceship->getPosition().y - spaceship_t->getSize().x < 0)
    spaceship->setPosition(spaceship->getPosition().x, spaceship_t->getSize().x);
  else if (spaceship->getPosition().y > window->getSize().y)
    spaceship->setPosition(spaceship->getPosition().x, window->getSize().y);
}

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
  // neste jogo, vamos instanciar usando ponteiros
  auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(1280, 720), "Space War 0.4", sf::Style::Titlebar | sf::Style::Close);
  window->setFramerateLimit(60);
  window->setPosition(sf::Vector2i(30, 30));

  // textures
  std::shared_ptr<sf::Texture> spaceship_t = std::make_shared<sf::Texture>();
  auto bullet_t = std::make_shared<sf::Texture>();
  auto enemy_t = std::make_shared<sf::Texture>();
  auto life_t = std::make_shared<sf::Texture>();
  auto background_t = std::make_shared<sf::Texture>();
  auto explosion_t = std::make_shared<sf::Texture>();
  // font
  auto font = std::make_shared<sf::Font>();

  try
  {
    if (!spaceship_t->loadFromFile("./assets/img/spaceship-min.png"))
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
  catch (const char *msg)
  {
    std::cerr << msg << '\n';
  }
  // spaceship
  std::shared_ptr<sf::Sprite> spaceship_spr = std::make_shared<sf::Sprite>();
  spaceship_spr->setTexture(*spaceship_t);
  spaceship_spr->setRotation(270.f); // rotaciono a nave pra deixar ela virada pra cima
  // eu pego o tamanho do y da minha nave pois ela originalmente é virada pra horizontal
  spaceship_spr->setPosition(static_cast<float>(window->getSize().x) / 2 - static_cast<float>(spaceship_t->getSize().y) / 2, 500.f);
  // bullet (perceba que eu vou instanciar ela de forma diferente da spaceship, por fins didáticos
  // eu vou precisar de um vetor, para dar multiplos tiros

  // bullet
  auto bullet_spr = std::make_shared<sf::Sprite>(*bullet_t);
  bullet_spr->setRotation(270.f);
  bullet_spr->setScale(0.1f, 0.1f);
  // eu nao vou criar o vetor como um ponteiro inteligente, isso por causa de um delay que existe nesses arrays, ja que é armazenado em forma de pilha
  std::vector<sf::Sprite> bullets, enemies;
  // limita os tiros e inimigos
  int shoot = 20, spawn_enemies = 1;

  // enemies
  auto enemy_spr = std::make_shared<sf::Sprite>(*enemy_t);
  // vamos setar a posicao do inimigo de forma randomica
  enemy_spr->setPosition(
      std::rand() % window->getSize().x + enemy_spr->getGlobalBounds().width,
      0);
  enemies.push_back(*enemy_spr);

  // life
  auto spritesheet_life = std::make_shared<sf::Sprite>(*life_t);
  spritesheet_life->setPosition(
      static_cast<float>(window->getSize().x) / 2 - static_cast<float>(life_t->getSize().x) / 2, 690.f);

  // nós precisamos apenas de uma parte dessa spritesheet, para isso, vamos fazer:
  spritesheet_life->setTextureRect(sf::IntRect(0, 0, 300, 30));
  int spritesheet_y = 0; // controla a posicao na spritesheet

  // score
  int score;
  auto score_txt = std::make_shared<sf::Text>();
  score_txt->setFont(*font);
  score_txt->setFillColor(sf::Color::White);
  score_txt->setPosition(30, 30);

  // background
  auto background_spr = std::make_shared<sf::Sprite>(*background_t);

  // gameover
  bool gameover = false;
  auto gameover_txt = std::make_shared<sf::Text>();
  gameover_txt->setFont(*font);
  gameover_txt->setFillColor(sf::Color::White);
  gameover_txt->setCharacterSize(150);
  gameover_txt->setString("GAME OVER");
  gameover_txt->setPosition(
      (float)(window->getSize().x) / 2 - 225.f, (float)(window->getSize().y) / 2 - 150.f);

  // pause
  bool paused = false;
  auto paused_txt = std::make_shared<sf::Text>();
  paused_txt->setFont(*font);
  paused_txt->setFillColor(sf::Color::White);
  paused_txt->setCharacterSize(150);
  paused_txt->setString("PAUSED");
  paused_txt->setPosition(
      (float)(window->getSize().x) / 2 - 135.f, (float)(window->getSize().y) / 2 - 150.f);

  // information text
  auto info_txt = std::make_shared<sf::Text>();
  info_txt->setFont(*font);
  info_txt->setFillColor(sf::Color::Yellow);
  info_txt->setCharacterSize(70);

  // explosion spritesheet
  auto explosion_spr = std::make_shared<sf::Sprite>(*explosion_t);
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
