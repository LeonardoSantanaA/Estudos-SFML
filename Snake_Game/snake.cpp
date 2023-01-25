#include "snake.hpp"
#include "game.hpp"
#include <SFML/System/Vector2.hpp>
#include <cstdlib>

SnakeGame s[100];
Fruit fruit;

//construtor
Snake::Snake()
{
  //tamanho da janela
  cols = 20;
  lines = 13;
  size = 64;
  width = size * cols; 
  height = size * lines;

  direction = 0; //direcao
  snake_size = 4; //tamanho da cobra
  timer = 0.f;
  delay = 0.1f;
  fruit.x = fruit.y = 10; //as frutas vao iniciar na coordanada (10, 10)
  points = 0;

  //criando a janela
  window.create(sf::VideoMode(width, height), "Snake Game", sf::Style::Titlebar | sf::Style::Close);
  window.setFramerateLimit(30);
  window.setPosition(sf::Vector2i(40, 40));
}

//functions
void Snake::verifyFiles()
{
  if(!background_t.loadFromFile("./assets/img/bg.png"))
    throw "Não foi possível carregar a imagem do background! Verifique a integridade dos seus arquivos.";
  if(!fruit_t.loadFromFile("./assets/img/fruta.png"))
    throw "Não foi possível carregar a imagem das frutas! Verifique a integridade dos seus arquivos.";
  if(!snake_t.loadFromFile("./assets/img/snake.png"))
    throw "Não foi possível carregar a imagem da snake! Verifique a integridade dos seus arquivos.";
  if(!font.loadFromFile("./assets/font/arial.ttf"))
    throw "Não foi possível carregar a font! Verifique a integridade dos seus arquivos.";

  background_spr.setTexture(background_t);
  fruit_spr.setTexture(fruit_t);
  snake_spr.setTexture(snake_t);
  txt.setFont(font);
  txt.setString("Pontos: " + std::to_string(points));
  txt.setFillColor(sf::Color::White);
  txt.setPosition(sf::Vector2f(10, 10));
}

void Snake::move()
{
  //loop que percorre e faz a cobra seguir
  for(int i = snake_size; i > 0; --i)
  {
    s[i].x = s[i - 1].x;
    s[i].y = s[i - 1].y;
  }

  //checando as teclas
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) && direction != 1)
   direction = 0;
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && direction != 0)
   direction = 1;
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) && direction != 3)
   direction = 2;
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && direction != 2)
   direction = 3;
    
  //controle de direcao
  switch(direction)
  {
   case 0:
    s[0].x += 1;
    break;

   case 1:
    s[0].x -= 1;
    break;

   case 2:
    s[0].y -= 1;
    break;

   case 3:
    s[0].y += 1;
    break;        
  }
}

void Snake::generateFruit()
{
  points++;
  fruit.x = std::rand() % cols;
  fruit.y = std::rand() % lines;
  if(s[0].x == fruit.x && s[0].y == fruit.y)
     generateFruit();
}

void Snake::collision()
{
  //colisao com a fruta
  if(s[0].x == fruit.x && s[0].y == fruit.y)
  {
    ++snake_size;
    generateFruit();
  }

  //colisao com as extremidades
  if(s[0].x > cols)
    s[0].x = 0;
  if(s[0].x < 0)
    s[0].x = cols;
   if(s[0].y > lines)
    s[0].y = 0;
  if(s[0].y < 0)
    s[0].y = lines;

  //colisao com a cauda
  for(int i = 1; i < snake_size; i++)
  {
   if(s[0].x == s[i].x && s[0].y == s[i].y)
   {
     window.close();
   }
  }
}

void Snake::runGame()
{
  try
  {
   this->verifyFiles();  
  }
  catch (const char* msg) 
  {
    std::cerr << "ERRO: " << msg;
    return;
  } 
  while( window.isOpen())
  {   
    float time = clock.getElapsedTime().asSeconds(); //vou armazenar o valor do relogio nessa variavel
    clock.restart(); //agora posso reiniciar ele, pois ja armazenei o valor
    timer += time; //e vou acrescentar na variavel timer
    sf::Event event;
    while(window.pollEvent(event))
    {
      if(event.type == sf::Event::Closed)
      {
        window.close();
      }
    }
    if(timer > delay)
    {
    this->move();
    this->collision();
    timer = 0;
    }
    window.clear();

    // loop aninhado para desenhar o background
    for(int i{}; i < cols; ++i)
    {
      for(int j{}; j < lines; ++j)
      {
        background_spr.setPosition( i * size, j * size);
        window.draw(background_spr);
      }
    }

    //loop que percorre pelo tamanho da cobra
    for(int i{}; i < snake_size; ++i)
    {
      snake_spr.setPosition(s[i].x * size, s[i].y * size);
      window.draw(snake_spr);
    }

    //desenhando a fruta
    fruit_spr.setPosition( fruit.x * size, fruit.y * size);
    window.draw(fruit_spr);

    //desenhando o score
    txt.setString("Pontos: " + std::to_string(points));
    window.draw(txt);  

    window.display();
  }
}
