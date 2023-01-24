#include <SFML/Graphics.hpp>
#include <memory.h>
#include <iostream>
#include <experimental/random>
#include <pthread.h>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1200, 600), "Minecrap");
    window.setFramerateLimit(60); //limite fps
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
   
    //vou precisar tambem de um vetor, pois sao varios objetos na tela
    std::vector<sf::RectangleShape> objs;
    size_t max_objs = 5; //qtd maxima de objetos que podem aparecer na tela 
    float obj_vel_max = 8.f;
    float obj_vel = obj_vel_max;

    //gerando valor randomico
    float x = static_cast<float>( std::experimental::randint(10, int(window.getSize().x - object.getSize().x) ) );
    object.setPosition(x, 10.f);

    //posicoes do mouse
    sf::Vector2i pos_mouse_win; //posicao do mouse em relacao a janela
    sf::Vector2f pos_mouse_coord; //vai armazenar as coordenadas mapeadas

    //score e vida
    int score = 0;
    int health = 3;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            pos_mouse_win = sf::Mouse::getPosition(window); //ficar sempre pegando a posicao do meu mouse
            pos_mouse_coord = window.mapPixelToCoords(pos_mouse_win); //converte a posicao do mouse em relacao a janela
        }

        if(objs.size() < max_objs) //adiciona os objetos no vetor
        {
          if(obj_vel >= obj_vel_max)
          {
            x = static_cast<float>( std::experimental::randint(10, int(window.getSize().x - object.getSize().x) ) ); //vou gerar o numero randomico novamente
            object.setPosition(x, 10.f);
            objs.push_back(object);
            obj_vel = 0.f;
          }else
          {
            obj_vel += 1.f;
          }
        }

        //mover e deletar os objetos do vetor
        for (size_t i{}; i < objs.size(); ++i)
        {
          bool del = false; //controla se vai ou nao deletar o obj
          objs[i].move(0.f, 5.f); //fazer o objeto sempre movimentar pra baixo

          if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) //verifica se clicou com o botao esquerdo
          {
            if(objs[i].getGlobalBounds().contains(pos_mouse_coord)) //essas funcoes verifica se na coordernada que eu cliquei com o botao do mouse, continha algum pixel do retangulo do objeto, 
                                                                 //fazendo assim uma verificacao de colisao
            {
              del = true;
              score += 10;
              std::cout << "Score: " << score << '\n';
            }
          }

          if(objs[i].getPosition().y > window.getSize().y) //verifica se saiu da tela por baixo
          {
            std::cout << "Vida: " << --health << '\n';
            del = true;
            //gameover
            if(health <= 0)
            {
              window.close();
            }
          }  

          if(del)
          {
            objs.erase(objs.begin() + i); //o erase() recebe um iterator, por isso eu tenho que pegar o primeiro obj pelo begin() que retorna um iterator, e somo i para ir pra posicao do obj que quero deletar
          }

        }

       
             

        window.clear();

        //foreach que percorre os objs para desenhar
        for (auto &e : objs)
        {
          window.draw(e);
        }

        window.display();
    }
   

    return EXIT_SUCCESS;
}
