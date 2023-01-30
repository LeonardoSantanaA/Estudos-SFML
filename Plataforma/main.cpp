#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

//map
//meu tilemap vai ser por caracteres
const int H = 23, W = 80 / 2;
sf::String tilemap[H] = {
"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
"B                                B                                       B     B", 
"B                                B                                       B     B",
"B                                B                                       B     B",
"B                                B                                       B     B",
"B                                B                                       B     B",
"B                                B                                       B     B",
"B                                B                                       B     B",
"B                                B                                       B     B",
"B                                B                                       B     B",
"B                                B                                       B     B",
"B                                B                                       B     B",
"B                                B                                       B     B",
"B                                B                                       B     B",
"B                                B                                       B     B",
"B         0 0 0 0                B                0000                BBBB     B",
"B                             BBBB                                       B     B",
"BBB                              B       BB                              B     B",
"B              BB         BB     BB    BB              BB              BBBB    B",
"B          0   BB         BB           BB              BB                      B",
"B    B         BB         BB           BB    B         BB         BB           B",
"B    B         BB        BBBB         BBB    B         BB         BB           B",
"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB"  
};

class Player
{
private:
  const int ground = 736 - (82 + 32); // 736 é o tamanho da altura da janela, 82 a altura da sprite e 32 a altura do chao (736 pra ficar certinho com o tamanho da sprite do chao)
  float dx, dy, frame;                 // dx controla os movimentos horizontais, dy os movimentos verticais, frame controla a spritesheet
  bool on_ground;                      // verifica se o player  esta no chao
  std::shared_ptr<sf::Sprite> sprite;
  const float player_speed = 0.4f;
  std::shared_ptr<sf::FloatRect> rect;

public:
  // Construtor
  Player(sf::Texture &texture)
  {
    sprite = std::make_shared<sf::Sprite>(texture);
    sprite->setTextureRect(sf::IntRect(0, 0, 43, 82)); // pego a parte da sprite pra desenhar
    rect = std::make_shared<sf::FloatRect>(64, ground, 43, 82);
    dx = 0.f;
    dy = 0.f;
    frame = 0.f;
    on_ground = false;
  }

  // Getters & Setters
  float getDx()
  {
    return this->dx;
  }

  void setDx(float dx)
  {
    this->dx = dx;
  }

  float getDy()
  {
    return this->dy;
  }

  void setDy(float dy)
  {
    this->dy = dy;
  }

  float getRectLeft()
  {
    return this->rect->left;
  }

  void setRectLeft(float rectLeft)
  {
    this->rect->left = rectLeft;
  }

  float getRectTop()
  {
    return this->rect->top;
  }

  void setRectTop(float rectTop)
  {
    this->rect->top = rectTop;
  }

  float getRectHeight()
  {
    return this->rect->height;
  }

    float getRectWidth()
  {
    return this->rect->width;
  }



  sf::Sprite getSprite()
  {
    return *this->sprite;
  }

  bool isOnGround()
  {
    return this->on_ground;
  }

  // update
  void update(float &time)
  {


    if (!isOnGround())
    {
      setDy(getDy() + 0.0008 * time);
      sprite->move(sprite->getPosition().x, dy);
    }

    on_ground = false; // pro player nao ficar voando 
    collision(0); 
    // essa verificacao é responsavel por fazer o nosso player nao passar do chao
    if (getRectTop() > ground)
    { // verifica se está no chao
      setRectTop(ground);
      setDy(0);
      on_ground = true; // eu nao quero controlar o on_ground por fora da classe, por isso estou mexendo na variavel diretamente sem get e set
    }

    frame += 0.01f * time; // nao quero controlar o frame por fora da classe, por isso estou mexendo na variavel diretamente
    if (frame > 6)
    {             // total de sprites que meu spritesheet possui (iniciando do 0)
      frame -= 6; // vou voltar pro 0
    }


    // movimento player
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
      setDx(player_speed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
      setDx(-player_speed);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    { // tecla de pulo
      if (isOnGround())
      {
        setDy(-player_speed);
      }
    }

    if (getDx() > 0) // utilizar os frames pra direita
    {
      sprite->setTextureRect(sf::IntRect(43 * (int)frame, 0, 43, 82)); // pego a parte da sprite pra desenhar
    }
    else if (getDx() < 0) // utilizar os frames pra esquerda
    {
      sprite->setTextureRect(sf::IntRect(43 * (int)frame + 43, 0, -43, 82)); // quando a largura é valor negativo (no caso -43, ele inverte a sprite, porem preciso de somar mais 1 sprite (por isso tem + 43 no primeiro parametro)
    }

    // para usar no setPosition() la embaixo
    // é mais simples usar o atributo rect diretamente nesse caso, por isso nao tem um get e um set pro rect
    setRectLeft(getRectLeft() + getDx() * time); // atualizacao dos valores do left do meu retangulo para realizar o movimento (rect.left) pega o primeiro parametro do retangulo que eu criei (left, top, width, height)
    collision(1);
    setRectTop(getRectTop() + getDy() * time);
    
    // de acordo com os valores verificados acima, eu vou setar a posicao do meu player
    sprite->setPosition(rect->left, rect->top);
    // vou zerar o dx pra ele nao ficar correndo sem parar
    setDx(0);
  }

void collision(int direction){
  for(int i = getRectTop() / 32; i < (getRectTop() + getRectHeight()) / 32; ++i){ //percore a vertical
      for(int j = getRectLeft() / 32; j < (getRectLeft() + getRectWidth()) / 32; ++j){  //percorre a horizontal
        if(tilemap[i][j] == 'B'){ //se colidir com algum B
          //colisao com o X
          if(direction == 1){
            if(getDx() > 0){  //e ele estiver movimentando pra direita
              setRectLeft(j * 32 - getRectWidth()); //vou setar a horizontal dele para aquela posicao
            }
            if(getDx() < 0){ //movimentando pra esquerda
              setRectLeft(j * 32 + 32);
            }
          }               
          //colisao com o Y
          if(direction == 0){
            if(getDy() > 0){  //e ele estiver movimentando pra baixo
              setRectTop(i * 32 - getRectHeight());
              setDy(0);
              on_ground = true;
            }
            if(getDy() < 0){ //pra cima
              setRectTop(i * 32 + 32);
              setDy(0);
            }
          }
        }
        //colisao com o item coletavel
        if(tilemap[i][j] == '0'){
          tilemap[i][j] = ' ';
        }
      }
    }

  }
};
int main()
{
  // vou iniciar a janela na stack também
  sf::RenderWindow window(sf::VideoMode(1280, 736), "Plataforma 2D", sf::Style::Titlebar | sf::Style::Close);
  window.setPosition(sf::Vector2i(30, 30));
  window.setFramerateLimit(120);
  // para este jogo, alem de travar em 120 FPS, vou usar o time para fazer uma pequena suavizacao dos movimentos

  // constantes que vou utilizar
  const float height_floor = 32.f;

  // iniciando texturas
  std::shared_ptr<sf::Texture> bg_t = std::make_shared<sf::Texture>();
  std::shared_ptr<sf::Texture> floor_t = std::make_shared<sf::Texture>();
  std::shared_ptr<sf::Texture> player_t = std::make_shared<sf::Texture>();

  // carregando arquivos
  if (!bg_t->loadFromFile("./assets/img/bg.jpg"))
  {
    std::cerr << "Não foi possível carregar a imagem do background! Verifique a integridade dos seus arquivos.\n";
    window.close();
  }
  if (!floor_t->loadFromFile("./assets/img/floormax.jpg"))
  {
    std::cerr << "Não foi possível carregar a imagem do chão! Verifique a integridade dos seus arquivos.";
    window.close();
  }
  if (!player_t->loadFromFile("./assets/img/afro.png"))
  {
    std::cerr << "Não foi possível carregar a imagem do player! Verifique a integridade dos seus arquivos.";
    window.close();
  }

  // iniciando sprites
  std::shared_ptr<sf::Sprite> bg_spr = std::make_shared<sf::Sprite>(*bg_t);
  std::shared_ptr<sf::Sprite> floor_spr = std::make_shared<sf::Sprite>(*floor_t);
  floor_spr->setPosition(0.f, window.getSize().y - height_floor);
  std::shared_ptr<sf::Sprite> player_spr = std::make_shared<sf::Sprite>();

  // iniciando player
  std::shared_ptr<Player> player = std::make_shared<Player>(*player_t);

  // time
  sf::Clock clock; // vou iniciar o clock na stack mesmo
  
  //criando o mapa
  std::shared_ptr<sf::RectangleShape> rectangle = std::make_shared<sf::RectangleShape>(sf::Vector2f(height_floor, height_floor)); //vou criar um quadrado


  while (window.isOpen())
  {
    float time = clock.getElapsedTime().asMicroseconds(); // dentro do loop, vou criar uma variavel que armazena o tempo em microssegundos
    clock.restart();                                      // como estou armazenando o tempo na variavel acima, vou reiniciar o relogio
    time /= 800;                                          // vou dividir o tempo por 800, isso vai dar um movimento mais suave ao game
    sf::Event event;

    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        window.close();
      }
    }

    player->update(time);

    window.clear(sf::Color::Yellow);
    window.draw(*bg_spr);
   
   //loop que verifica o tilemap percorrendo pelo vetor de string criado no topo
    for(int i{}; i < H; ++i){
      for(int j{}; j < W; ++j){
        if(tilemap[i][j] == 'B'){ //quando o caractere for B, vai ser os colisores dos cantos
          rectangle->setFillColor(sf::Color::Black);
        }
        if(tilemap[i][j] == '0'){
          rectangle->setFillColor(sf::Color::Blue);
        }
        if(tilemap[i][j] == ' '){ //quando for vazio, nao fazer nada
          continue;
        }

        rectangle->setPosition(j * 32, i * 32); //colocar um retangulo na posicao
        window.draw(*rectangle);
      }
    }
  
    window.draw(*floor_spr);
    window.draw(player->getSprite());
    window.display();
  }

  return EXIT_SUCCESS;
}
