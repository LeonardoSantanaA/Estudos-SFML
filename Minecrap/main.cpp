#include <SFML/Graphics.hpp>
#include <memory.h>
#include <iostream>
#include <experimental/random>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Minecrap");
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
    
    //nesse caso, vou usar a forma "tradicional" e alocar na stack
    sf::RectangleShape object;
    object.setSize(sf::Vector2f(100.f, 100.f));
    object.setFillColor(sf::Color(0, 255, 0));
    
    //gerando valor randomico
    float x = static_cast<float>( std::experimental::randint(10, int(window.getSize().x - object.getSize().x) ) );
    object.setPosition(x, 10.f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        object.move(0.f, 0.02f); //fazer o objeto sempre movimentar pra baixo
        if(object.getPosition().y > window.getSize().y) //verifica se saiu da tela por baixo
        {
          x = static_cast<float>( std::experimental::randint(10, int(window.getSize().x - object.getSize().x) ) ); //vou gerar o numero randomico novamente
          object.setPosition(x, 10.f);
        }       

        window.clear();
        window.draw(object);
        window.display();
    }
   

    return EXIT_SUCCESS;
}
