#ifndef MINECRAP_H
#define MINECRAP_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>
#include <memory.h>
#include <iostream>
#include <experimental/random>

class Minecrap
{
public:
  // functions
  Minecrap();
  void verifyFolders();
  void codeGame();
  void gameOver();
  void pauseGame();
  void loopEvents();
  void runGame();

  sf::RenderWindow window; // o ultimo parametro impede o redimensionamento da janela

  // musica e sonoplastia
  sf::SoundBuffer pop_buffer, gameover_buffer;
  sf::Sound pop_snd, gameover_snd;
  sf::Music music;

  // gameplay
  bool paused, gameover, pressed;
  float velocity, x;
  int health, points;
  /*
  //criacao do retangulo, posso criar usando um ponteiro, para utilizar ponteiro eu preciso passar um valor para armazenar na heap
  //sempre que criar um ponteiro, lembre-se de destrui-lo!
  sf::RectangleShape * object = new sf::RectangleShape();
  object->setSize(sf::Vector2f(100.f, 100.f));
  object->setFillColor(sf::Color(0, 255, 0));
  object->setPosition(sf::Vector2f(10.f, 10.f));
  para deletar o ponteiro, no final (apos o while) eu colocaria:
  delete object; //deleto o ponteiro, porem isso ainda deixa resquicio no computador do usuario, por isso vou setar o valor do meu objeto como nullptr (ponteiro nulo):
  object = nullptr;
  */

  /*
  //ou entao, posso usar ponteiro inteligente para nao correr o risco de deixar memoria vazada no pc do usuario:
  auto object = std::make_shared<sf::RectangleShape>();
  object->setSize(sf::Vector2f(100.f, 100.f));
  object->setFillColor(sf::Color(0, 255, 0));
  object->setPosition(sf::Vector2f(10.f, 10.f));
  */

  /*
  //posso tabem  usar a forma "tradicional" e alocar na stack
  sf::RectangleShape object;
  object.setSize(sf::Vector2f(100.f, 100.f));
  object.setFillColor(sf::Color(0, 255, 0));
  */

  // Font
  sf::Font pixel_fnt, jet_fnt;
  sf::Text score_text, life_text, gameover_text, paused_text, info_text;

  // Texturas e Sprites
  sf::Texture texture, bg, pickaxe, paused_img;
  sf::Sprite object, background, mouse, paused_spr;

  // vou precisar tambem de um vetor, pois sao varios objetos na tela
  std::vector<sf::Sprite> objs;
  size_t max_objs; // qtd maxima de objetos que podem aparecer na tela

  // posicoes do mouse
  sf::Vector2i pos_mouse_win;   // posicao do mouse em relacao a janela
  sf::Vector2f pos_mouse_coord; // vai armazenar as coordenadas mapeadas
};

#endif
