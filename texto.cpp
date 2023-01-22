#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
  sf::RenderWindow window(sf::VideoMode(800, 600), "Textos");
  sf::Event event;

  // para trabalhar com texto, primeiro precisamos de uma font:
  sf::Font font;
  if (!font.loadFromFile("./PixelBloated.ttf")) // vamos tentar carregar a font, se nao conseguir vai soltar um erro e sair
  {
    std::cerr << "Falha ao ler a fonte\n";
    return EXIT_FAILURE;
  }

  int score = 0;
  bool pressed = false; // vai fazer eu dar apenas um clique com o esquerdo, e preciso clicar com o direito depois para conseguir marcar mais um no score
  sf::Text text("Score: " + score, font, 28);
  sf::Color rgb(55, 44, 77);
  text.setFillColor(rgb);

  while (window.isOpen())
  {
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        window.close();
      }

      if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && pressed) // se eu apertar o botao direito e pressed for true
      {
        pressed = !pressed; // se for falso vira verdadeiro, se for verdadeiro vira falso
      }

      if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && sf::Mouse::isButtonPressed(sf::Mouse::Left)) // verifica se esta apertando os dois botoes do mouse
      {
        pressed = true;
      }

      if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !pressed)
      {
        score++;
        text.setString("Score: " + std::to_string(score));
        pressed = true;
      }
    }

    window.clear();
    window.draw(text);
    window.display();
  }

  return EXIT_SUCCESS;
}