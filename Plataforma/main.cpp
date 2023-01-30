#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

int main(){
  sf::RenderWindow window(sf::VideoMode(1280, 720), "Plataforma 2D", sf::Style::Titlebar | sf::Style::Close);
  window.setPosition(sf::Vector2i(30, 30));
  window.setFramerateLimit(60);

  //FLOOR
  sf::RectangleShape floor(sf::Vector2f(window.getSize().x, 60.f));
  floor.setPosition(0.f ,window.getSize().y - 60.f);
  floor.setFillColor(sf::Color::Black);

  //PLAYER
  sf::Texture player_t;
  if(!player_t.loadFromFile("./assets/img/afro.png"))
     throw "Não foi possível carregar a imagem do player! Verifique a integridade dos seus arquivos.";
  sf::Sprite player_spr(player_t, sf::IntRect(0, 0, 43, 82));
  player_spr.setPosition(0, window.getSize().y - player_spr.getGlobalBounds().height - 60.f);

  while(window.isOpen()){
    sf::Event event;

    while(window.pollEvent(event)){
      if(event.type == sf::Event::Closed){
        window.close();
      }
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
      player_spr.move(8.f, 0);
    } 
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
      player_spr.move(-8.f, 0);
    }

    window.clear(sf::Color::Yellow);
    window.draw(floor);
    window.draw(player_spr);
    window.display();
  }
 
  return EXIT_SUCCESS;
}

