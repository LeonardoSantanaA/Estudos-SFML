#ifndef GAME_H_
#define GAME_H_

#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include "ship.hpp"

class Game
{
public:
  // neste jogo, vamos instanciar usando ponteiros
  // janela do game
  std::shared_ptr<sf::RenderWindow> window;
  // textures
  std::shared_ptr<sf::Texture> ship_t;
  std::shared_ptr<sf::Texture> bullet_t;
  std::shared_ptr<sf::Texture> enemy_t;
  std::shared_ptr<sf::Texture> life_t;
  std::shared_ptr<sf::Texture> background_t;
  std::shared_ptr<sf::Texture> explosion_t;
  // font
  std::shared_ptr<sf::Font> font;
  // sprites
  std::shared_ptr<sf::Sprite> ship_spr;
  std::shared_ptr<sf::Sprite> enemy_spr;
  std::shared_ptr<sf::Sprite> bullet_spr;
  std::shared_ptr<sf::Sprite> spritesheet_life;
  std::shared_ptr<sf::Sprite> background_spr;
  std::shared_ptr<sf::Sprite> explosion_spr;
  // texts
  std::shared_ptr<sf::Text> score_txt;
  std::shared_ptr<sf::Text> gameover_txt;
  std::shared_ptr<sf::Text> paused_txt;
  std::shared_ptr<sf::Text> info_txt;
  // objects
  std::shared_ptr<Ship> ship;

  Game();
  void loadFiles();
  void startGame();
};

#endif