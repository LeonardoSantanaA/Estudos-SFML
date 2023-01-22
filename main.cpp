#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>

int main()
{
  sf::String title = "Fundamentos Iniciais"; // perceba que estamos usando apenas o namespace do sf, nao estamos usando std.
  sf::RenderWindow window;                   // renderizar a janela, windows é o nome do objeto
  sf::VideoMode dimension(800, 600);         // cria a janela com seu tamanho
  window.create(dimension, title);           // inicio a minha janela, o primeiro paraametro é a dimensao que criei a cima e o segundo o nome que vai aparecer na janela
  /*
    ou entao, se nao quiser criar uma janela instanciando esse monte de coisa, voce pode fazer simplesmente:
    sf::RenderWindow window(sf::VideoMode(800, 600), "Fundamentos Iniciais");
  */

  // desenhando um circulo na janela:
  sf::CircleShape circle(100.f); // o parametro é o raio

  while (window.isOpen()) // o loop é necessario pois se eu abrir a janela e o codigo acabar, a janela fecha, entao o loop nao vai deixar a janela fechar instantaneamente, isOpen() é um booleano
                          // que verifica se a janela está aberta
  {
    sf::Event event;                // o objeto Event é usado para captar os eventos do usuario, como clique do mouse, clique de alguma tecla etc
    while (window.pollEvent(event)) // pollEvent() é a funcao que permite ler todos os movimentos do usuario, o evento é lido e convertido para numeros, cada numero é determinado evento, por exemplo, o evento que
    {                               // o usuario clica para fechar janela é o 0, mas ao inves de fazer if(event.type == 0), eu posso usar:
      if (event.type == sf::Event::Closed)
      {
        window.close();
      }
    }
    // std::cout << "O tipo de evento é: " << event.type << '\n';
    window.clear(sf::Color::Cyan); // limpa a tela
    // window.draw(circle);
    window.display(); // mostrar a janela
  }

  return EXIT_SUCCESS;
}
