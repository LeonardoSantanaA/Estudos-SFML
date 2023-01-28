#ifndef GAME_H_
#define GAME_H_

#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cstdlib>
#include "ship.hpp"
#include "enemy.hpp"

class Game
{
private:
  // neste jogo, vamos instanciar usando ponteiros
  // janela do game
  std::shared_ptr<sf::RenderWindow> window;
  // gameplay
  int spritesheet_y, score;
  bool gameover, paused;
  float frame, speedFrame;
  float count, max; // quantidade de frames, max é para limitar a quantidade de frames
  bool show;        // mostrar a animacao apenas uma vez
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
  std::shared_ptr<Enemy> enemy;
  // sounds
  sf::Music music;

  void
  loadFiles();
  void flowGame();
  void gameOver();
  void pause();

  void collision();
  void animationExplosion();

public:
  Game();
  void startGame();
};

#endif