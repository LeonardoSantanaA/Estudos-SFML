#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

class Player
{
private:
  const int ground = 720 - (82 + 100); // 720 é o tamanho da altura da janela, 82 a altura da sprite e 100 a altura do chao
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
    sprite->setTextureRect(sf::IntRect(43, 0, 43, 82)); // pego a parte da sprite pra desenhar
    rect = std::make_shared<sf::FloatRect>(0, ground, 43, 82);
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

  float setRectLeft(float rectLeft)
  {
    this->rect->left = rectLeft;
  }

  float getRectTop()
  {
    return this->rect->top;
  }

  float setRectTop(float rectTop)
  {
    this->rect->top = rectTop;
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
      setDy(getDy() + 0.0006 * time);
      sprite->move(sprite->getPosition().x, dy);
    }

    on_ground = false; // pro player nao ficar voando

    // essa verificacao é responsavel por fazer o nosso player nao passar do chao
    if (getRectTop() > ground)
    { // verifica se está no chao
      setRectTop(ground);
      setDy(0);
      on_ground = true; // eu nao quero controlar o on_ground por fora da classe, por isso estou mexendo na variavel diretamente sem get e set
    }

    frame += 0.4f * time; // nao quero controlar o frame por fora da classe, por isso estou mexendo na variavel diretamente
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
    setRectTop(getRectTop() + getDy() * time);

    // de acordo com os valores verificados acima, eu vou setar a posicao do meu player
    sprite->setPosition(rect->left, rect->top);
    // vou zerar o dx pra ele nao ficar correndo sem parar
    setDx(0);
  }
};

int main()
{
  // vou iniciar a janela na stack também
  sf::RenderWindow window(sf::VideoMode(1280, 720), "Plataforma 2D", sf::Style::Titlebar | sf::Style::Close);
  window.setPosition(sf::Vector2i(30, 30));
  // window.setFramerateLimit(120);
  // para este jogo, alem de travar em 120 FPS, vou usar o time para fazer uma pequena suavizacao dos movimentos

  // constantes que vou utilizar
  const float height_floor = 100.f;

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
    window.draw(*floor_spr);
    window.draw(player->getSprite());
    window.display();
  }

  return EXIT_SUCCESS;
}
