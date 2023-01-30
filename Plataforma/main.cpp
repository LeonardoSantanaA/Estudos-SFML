#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <iostream>

int main(){
  sf::RenderWindow window(sf::VideoMode(1280, 720), "Plataforma 2D", sf::Style::Titlebar | sf::Style::Close);
  window.setPosition(sf::Vector2i(30, 30));
  window.setFramerateLimit(60);

  const float height_floor = 100.f;

  //FRAMES SPRITESHEET
  float frame = 0.f;

  //BACKGROUND
  sf::Texture bg_t;
  if(!bg_t.loadFromFile("./assets/img/bg.jpg"))
      throw "Não foi possível carregar a imagem do background! Verifique a integridade dos seus arquivos.";
  sf::Sprite bg_spr(bg_t);

  //FLOOR
  sf::Texture floor_t;
  if(!floor_t.loadFromFile("./assets/img/floormax.jpg"))
    throw "Não foi possível carregar a imagem do chão! Verifique a integridade dos seus arquivos.";
  sf::Sprite floor_spr(floor_t);
  //sf::RectangleShape floor(sf::Vector2f(window.getSize().x, height_floor));
  floor_spr.setPosition(0.f ,window.getSize().y - height_floor);
  //floor.setFillColor(sf::Color::Black);

  //PLAYER
  sf::Texture player_t;
  if(!player_t.loadFromFile("./assets/img/afro.png"))
     throw "Não foi possível carregar a imagem do player! Verifique a integridade dos seus arquivos.";
  sf::Sprite player_spr(player_t, sf::IntRect(0, 0, 43, 82));
  player_spr.setPosition(0, window.getSize().y - player_spr.getGlobalBounds().height - height_floor);

  while(window.isOpen()){
    sf::Event event;

    while(window.pollEvent(event)){
      if(event.type == sf::Event::Closed){
        window.close();
      }
    }

    //MOVIMENTO PLAYER
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
      player_spr.move(8.f, 0);
      frame += 0.4f;
      if(frame > 6){ //total de sprites que meu spritesheet possui (iniciando do 0)
        frame -= 6; //vou voltar pro 0
      }
      player_spr.setTextureRect(sf::IntRect(43 * (int)frame, 0, 43, 82)); //pego a parte da sprite pra desenhar
    } 
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
      player_spr.move(-8.f, 0);
      frame += 0.4f;
      if(frame > 6){ //total de sprites que meu spritesheet possui (iniciando do 0)
        frame -= 6; //vou voltar pro 0
      }  
      player_spr.setTextureRect(sf::IntRect(43 * (int)frame + 43, 0, -43, 82)); //quando a largura é valor negativo (no caso -43, ele inverte a sprite, porem preciso de somar mais 1 sprite (por isso tem + 43 no primeiro parametro)
     
    }

    window.clear(sf::Color::Yellow);
    window.draw(bg_spr);
    window.draw(floor_spr);
    window.draw(player_spr);
    window.display();
  }
 
  return EXIT_SUCCESS;
}

