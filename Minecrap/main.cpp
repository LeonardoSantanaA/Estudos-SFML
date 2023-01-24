#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/WindowStyle.hpp>
#include <cstdlib>
#include <memory.h>
#include <iostream>
#include <experimental/random>
#include <string>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1200, 600), "Minecrap", sf::Style::Titlebar | sf::Style::Close); // o ultimo parametro impede o redimensionamento da janela
    window.setFramerateLimit(60); //limite fps
    window.setPosition(sf::Vector2i(30, 30)); //posicao inicial da janela
    window.setMouseCursorVisible(false); //deixa o mouse invisivel
    
    bool canPress = true;
    bool pressed = false; //verifica se o jogador esta com o botao do mouse pressionado
    float velocity = 2.f;
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

    //Font
    sf::Font font;
    if(!font.loadFromFile("./Minecraft.ttf"))
    {
      std::cerr << "Erro ao carregar a font! Consulte a integridade dos seus arquivos." << std::endl;
      return EXIT_FAILURE;
    }
    sf::Text score, life; 

    //score
    int points = 0;
    score.setFont(font);
    score.setString("Score: " + std::to_string(points));
    score.setFillColor(sf::Color::White);
    score.setPosition(5.f, 5.f);

    //life 
    int health = 5;
    life.setFont(font);
    life.setString("Life: " + std::to_string(health));
    life.setFillColor(sf::Color::White);
    life.setPosition(1090.f, 5.f);

    //Texturas e Sprites
    sf::Texture texture, bg, pickaxe;
    if(!texture.loadFromFile("./spr_obj.png"))
    {
      std::cerr << "Erro ao carregar a textura do bloco! Consulte a integridade dos seus arquivos." << std::endl;
      return EXIT_FAILURE;
    }
    if(!bg.loadFromFile("./fundo.jpg"))
    {
      std::cerr << "Erro ao carregar a textura do background! Consulte a integridade dos seus arquivos." << std::endl;
      return EXIT_FAILURE;
    }
    if(!pickaxe.loadFromFile("./pickaxe.png"))
    {
      std::cerr << "Erro ao carregar a textura do mouse! Consulte a integridade dos seus arquivos." << std::endl;
      return EXIT_FAILURE;
    }

    sf::Sprite object(texture);
    sf::Sprite background(bg);
    sf::Sprite mouse(pickaxe);
  
   
    //vou precisar tambem de um vetor, pois sao varios objetos na tela
    std::vector<sf::Sprite> objs;
    size_t max_objs = 5; //qtd maxima de objetos que podem aparecer na tela 
    float obj_vel_max = 10.f;
    float obj_vel = obj_vel_max;

    //gerando valor randomico
    float x = static_cast<float>( std::experimental::randint(10, int(window.getSize().x - texture.getSize().x) ) );
    object.setPosition(x, 10.f);

    //posicoes do mouse
    sf::Vector2i pos_mouse_win; //posicao do mouse em relacao a janela
    sf::Vector2f pos_mouse_coord; //vai armazenar as coordenadas mapeadas

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            //tratamento da janela
            if(event.type == sf::Event::Resized)  //evento que deteca se o usuario redimensionou a janela
            {
              sf::FloatRect visible_area(0, 0, event.size.width, event.size.height); //cria um novo retangulo, que vai ser nossa area visivel, o tamanho é com base no tanto que o usuario redimensionou
              window.setView(sf::View(visible_area)); //setar a visibilidade nova
            }
            
            //tratamento da jogabilidade
            if(event.type == event.MouseButtonPressed)
            {
              pressed = false;
            }

            pos_mouse_win = sf::Mouse::getPosition(window); //ficar sempre pegando a posicao do meu mouse
            pos_mouse_coord = window.mapPixelToCoords(pos_mouse_win); //converte a posicao do mouse em relacao a janela
            mouse.setPosition((sf::Vector2f)pos_mouse_win); //a posicao da minha picareta (mouse) 
        }

        if(objs.size() < max_objs) //adiciona os objetos no vetor
        {
          if(obj_vel >= obj_vel_max)
          {
            x = static_cast<float>( std::experimental::randint(10, int(window.getSize().x - texture.getSize().x) ) ); //vou gerar o numero randomico novamente
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
          objs[i].move(0.f, velocity); //fazer o objeto sempre movimentar pra baixo

          if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && !pressed) //verifica se clicou com o botao esquerdo
          {
            if(objs[i].getGlobalBounds().contains(pos_mouse_coord)) //essas funcoes verifica se na coordernada que eu cliquei com o botao do mouse, continha algum pixel do retangulo do objeto, 
                                                                 //fazendo assim uma verificacao de colisao
            {
              pressed = true;
              del = true;
              points += 10;
              score.setString("Score: " + std::to_string(points));
              if(points % 100 == 0)
              {
                velocity += 0.1f;
                std::cout << "Nova velocidade: " << velocity << std::endl;
              }
              if(points % 1000 == 0)
              {
                ++health;
                std::cout << "Voce ganhou uma vida: " << health << std::endl;
                life.setString("Life: " + std::to_string(health));
              }
            }
          }

          if(objs[i].getPosition().y > window.getSize().y) //verifica se saiu da tela por baixo
          {
            --health;
            life.setString("Life: " + std::to_string(health));
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
        window.draw(background);
        window.draw(score);
        window.draw(life);
        //foreach que percorre os objs para desenhar
        for (auto &e : objs)
        {
          window.draw(e);
        }
        window.draw(mouse);
        window.display();
    }
   

    return EXIT_SUCCESS;
}
