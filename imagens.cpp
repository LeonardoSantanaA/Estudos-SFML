#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
  sf::RenderWindow window(sf::VideoMode(800, 600), "Imagens e Movimento");
  sf::Event event;

  // para usar imagens e mover elas, precisamos de dois objetos, um Texture e um Sprite.
  sf::Texture texture;                     // crio o objeto
  sf::Texture campo;                       // vou criar outro objeto Texture que vai ser o fundo
  if (!texture.loadFromFile("./ball.png")) // tento carregar a imagem
  {
    std::cerr << "Nao foi possivel carregar a imagem da bola\n";
    return EXIT_FAILURE;
  }
  if (!campo.loadFromFile("./campo.jpg"))
  {
    std::cerr << "Não foi possivel carregar a imagem de fundo\n";
    return EXIT_FAILURE;
  }

  sf::Sprite sprite(texture); // crio uma sprite e vou passar uma imagem (nossa textura)
  sf::Sprite fundo(campo);
  sprite.setPosition(sf::Vector2f(0.f, 200.f));
  // float speed = .01f;
  auto size = texture.getSize();     // o retorno do tamanho é um Vector2u (vetor inteiro sem sinal), porem vou usar o auto mesmo
  sf::Vector2f speed(0.02f, 0.02f);  // vou criar um vetor que vai ser a velocidade do meu x e do meu y
  sf::Vector2f pos_text(0.f, -50.f); // esse vetor vai ser somado a minha posicao do texto,e subtraindo do y eu consigo fazer subir um pouco o texto

  sf::Font font;
  if (!font.loadFromFile("./arial.ttf"))
  {
    std::cerr << "Não foi possivel carregar a font!\n";
    return EXIT_FAILURE;
  }
  sf::Text text("0, 200", font);
  text.setFillColor(sf::Color::Blue);

  while (window.isOpen())
  {
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        window.close();
      }
    }

    if ((sprite.getPosition().x + size.x > window.getSize().x && speed.x > 0) ||
        (sprite.getPosition().x < 0 && speed.x < 0)) // se bater nas paredes
    {
      speed.x = -speed.x; // perceba que eu falo se bater, o x vai ser o x negativo, se for positivo vai ser negativo mesmo, porem se for negativo vira negativo, ou seja, eu inverto os angulos (o mesmo pro y)
      sprite.setColor(sf::Color::Red);
    }

    if ((sprite.getPosition().y + size.x > window.getSize().y && speed.y > 0) ||
        (sprite.getPosition().y < 0 && speed.y < 0)) // se bater nas paredes
    {
      speed.y = -speed.y; // lembre-se que estamos trabalho com vetores no grafico, um vetor apenas indica a posicao que um objeto deve andar, nao vai se teleportar pra la!
      sprite.setColor(sf::Color::Green);
    }

    // sprite.setPosition(sprite.getPosition().x + speed, sprite.getPosition().y); // vou fazer minha sprite andar pra frente
    sprite.setPosition(sprite.getPosition() + speed); // agora estou passando só um parametro, isso pois agora estou usando um vector2
    text.setPosition(sprite.getPosition() + speed + pos_text);
    text.setString(std::to_string((int)sprite.getPosition().x) + "," + std::to_string((int)sprite.getPosition().y));

    window.clear();
    window.draw(fundo);
    window.draw(sprite);
    window.draw(text);
    window.display();
  }

  return EXIT_SUCCESS;
}