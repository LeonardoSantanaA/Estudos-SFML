#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
  sf::RenderWindow window(sf::VideoMode(800, 600), "Formas");
  sf::Event event;
  sf::RectangleShape rectangle(sf::Vector2f(100.f, 200.f)); // esse vetor2f é o tamanho do meu retangulo, 100 de largura e 200 de altura
  rectangle.setFillColor(sf::Color::Green);                 // setar a cor dele
  rectangle.setPosition(
      sf::Vector2f((static_cast<float>(window.getSize().x) / 2) - rectangle.getSize().x / 2,
                   ((float)window.getSize().y / 2) - rectangle.getSize().y / 2)); // vou setar a posicao para o meio da janela (static_cast<tipo>(valor) e (float)valor dão no mesmo)

  while (window.isOpen())
  {
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        window.close();
      }
    }

    auto position = sf::Mouse::getPosition(window); // vou pegar a posicao do meu mouse o parametro do getPosition() é que eu quero a posicao do meu mouse em relacao a minha janela (window)
    // rectangle.setPosition(sf::Vector2f(position)); //vou setar a posicao aqui, posso fazer assim tambem:
    // rectangle.setPosition(position.x, position.y); //da no mesmo

    if (position.x < window.getSize().x - rectangle.getSize().x && position.x > 0 && position.y < window.getSize().y - rectangle.getSize().y && position.y > 0) // para nao deixar passar da janela
    {
      rectangle.setPosition(sf::Vector2f(position));
    }

    if (position.x > window.getSize().x / 2)
    {
      rectangle.setFillColor(sf::Color::Red);
    }
    else
    {
      rectangle.setFillColor(sf::Color::Green);
    }

    window.clear();
    window.draw(rectangle);
    window.display();
    // std::cout << "posicao atual mouse: (" << position.x << ", " << position.y << ")\n";
  }

  return EXIT_SUCCESS;
}
