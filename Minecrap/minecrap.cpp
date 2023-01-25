#include "minecrap.hpp"

Minecrap::Minecrap()
{
  // window
  window.create(sf::VideoMode(1200, 600), "Minecrap", sf::Style::Titlebar | sf::Style::Close);
  window.setFramerateLimit(60);             // limite fps
  window.setPosition(sf::Vector2i(30, 30)); // posicao inicial da janela
  window.setMouseCursorVisible(false);      // deixa o mouse invisivel

  // music & sound effects
  sf::Sound pop_snd(pop_buffer), gameover_snd(gameover_buffer);
  sf::Music music;

  // gameplay
  paused = false;
  gameover = false;
  pressed = false; // verifica se o jogador esta com o botao do mouse pressionado
  velocity = 2.f;

  // random value
  x = static_cast<float>(std::experimental::randint(10, int(window.getSize().x - texture.getSize().x)));
  object.setPosition(x, 10.f);

  // information text
  info_text.setFont(jet_fnt);
  info_text.setFillColor(sf::Color::Yellow);

  // gameover
  gameover_text.setFont(pixel_fnt);
  gameover_text.setFillColor(sf::Color::White);
  gameover_text.setString("GAME OVER");
  gameover_text.setPosition(290, 200);
  gameover_text.setCharacterSize(100);

  // paused
  paused_text.setFont(pixel_fnt);
  paused_text.setFillColor(sf::Color::White);
  paused_text.setString("PAUSE");
  paused_text.setPosition(350, 200);
  paused_text.setCharacterSize(150);

  // score
  points = 0;
  score_text.setFont(pixel_fnt);
  score_text.setString("Score: " + std::to_string(points));
  score_text.setFillColor(sf::Color::White);
  score_text.setPosition(5.f, 5.f);

  // life
  health = 5;
  life_text.setFont(pixel_fnt);
  life_text.setString("Life: " + std::to_string(health));
  life_text.setFillColor(sf::Color::White);
  life_text.setPosition(1090.f, 5.f);

  // vou precisar tambem de um vetor, pois sao varios objetos na tela
  max_objs = 5; // qtd maxima de objetos que podem aparecer na tela
                //  float obj_vel_max = 10.f;
                //  float obj_vel = obj_vel_max;
}

// functions
void Minecrap::verifyFolders()
{
  // music & sound effects
  if (!pop_buffer.loadFromFile("./assets/sound/pop.wav"))
    throw "Erro ao carregar um efeito sonoro! Consulte a integridade dos seus arquivos.";

  if (!gameover_buffer.loadFromFile("./assets/sound/game-over-sound.wav"))
    throw "Erro ao carregar um efeito sonoro! Consulte a integridade dos seus arquivos.";

  if (!music.openFromFile("./assets/sound/soundtrack.wav"))
    throw "Erro ao carregar a soundtrack! Consulte a integridade dos seus arquivos.";

  // fonts
  if (!pixel_fnt.loadFromFile("./assets/fonts/Minecraft.ttf"))
    throw "Erro ao carregar a font! Consulte a integridade dos seus arquivos.";

  if (!jet_fnt.loadFromFile("./assets/fonts/jetbrains.ttf"))
    throw "Erro ao carregar a font! Consulte a integridade dos seus arquivos.";

  // textures & sprites
  if (!texture.loadFromFile("./assets/img//spr_obj.png"))
    throw "Erro ao carregar a textura do bloco! Consulte a integridade dos seus arquivos.";

  if (!bg.loadFromFile("./assets/img/fundo.jpg"))
    throw "Erro ao carregar a textura do background! Consulte a integridade dos seus arquivos.";

  if (!pickaxe.loadFromFile("./assets/img/pickaxe.png"))
    throw "Erro ao carregar a textura do mouse! Consulte a integridade dos seus arquivos.";

  if (!paused_img.loadFromFile("./assets/img/paused.jpg"))
    throw "Erro ao carregar a imagem de paused! Consulte a integridade dos seus arquivos.";

  // sound effects
  pop_snd.setBuffer(pop_buffer);
  gameover_snd.setBuffer(gameover_buffer);

  // textures & sprites
  object.setTexture(texture);
  background.setTexture(bg);
  mouse.setTexture(pickaxe);
  paused_spr.setTexture(paused_img);
}

void Minecrap::codeGame()
{
  if (objs.size() < max_objs) // adiciona os objetos no vetor
  {
    x = static_cast<float>(std::experimental::randint(10, int(window.getSize().x - texture.getSize().x))); // vou gerar o numero randomico novamente
    object.setPosition(x, 10.f);
    objs.push_back(object);
  }

  // mover e deletar os objetos do vetor
  for (size_t i{}; i < objs.size(); ++i)
  {
    bool del = false;            // controla se vai ou nao deletar o obj
    objs[i].move(0.f, velocity); // fazer o objeto sempre movimentar pra baixo

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !pressed) // verifica se clicou com o botao esquerdo
    {
      if (objs[i].getGlobalBounds().contains(pos_mouse_coord)) // essas funcoes verifica se na coordernada que eu cliquei com o botao do mouse, continha algum pixel do retangulo do objeto, fazendo assim uma verificacao de colisao
      {
        pop_snd.play();
        pressed = true;
        del = true;
        points += 10;
        score_text.setString("Score: " + std::to_string(points));
        if (points % 100 == 0)
        {
          velocity += 0.1f;
          std::cout << "Nova velocidade: " << velocity << std::endl;
        }
        if (points % 1000 == 0)
        {
          ++health;
          std::cout << "Voce ganhou uma vida: " << health << std::endl;
          life_text.setString("Life: " + std::to_string(health));
        }
      }
    }

    if (objs[i].getPosition().y > window.getSize().y) // verifica se saiu da tela por baixo
    {
      --health;
      life_text.setString("Life: " + std::to_string(health));
      del = true;
      // gameover
      if (health <= 0)
      {
        gameover_snd.play();
        gameover = true;
      }
    }

    if (del)
    {
      objs.erase(objs.begin() + i); // o erase() recebe um iterator, por isso eu tenho que pegar o primeiro obj pelo begin() que retorna um iterator, e somo i para ir pra posicao do obj que quero deletar
    }
  }

  window.clear();
  window.draw(background);
  window.draw(score_text);
  window.draw(life_text);
  // foreach que percorre os objs para desenhar
  for (auto &e : objs)
  {
    window.draw(e);
  }
  window.draw(mouse);
  window.display();
}
void Minecrap::gameOver()
{
  music.stop();
  info_text.setPosition(320, 310);
  info_text.setString("Aperte R para tentar novamente");
  window.setMouseCursorVisible(true);
  window.clear(sf::Color::Black);
  window.draw(score_text);
  window.draw(info_text);
  window.draw(gameover_text);
  window.display();

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
  {
    objs.clear();
    health = 5;
    life_text.setString("Life: " + std::to_string(health));
    points = 0;
    score_text.setString("Score: " + std::to_string(points));
    window.setMouseCursorVisible(false);
    velocity = 2.f;
    gameover = false;
  }
}
void Minecrap::pauseGame()
{
  music.pause();
  info_text.setPosition(370, 360);
  info_text.setString("Aperte ESC para retornar");
  window.clear();
  window.draw(paused_spr);
  window.draw(paused_text);
  window.draw(info_text);
  window.display();
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
  {
    paused = false;
  }
}
void Minecrap::loopEvents()
{
  sf::Event event;

  if (music.getStatus() == sf::Music::Stopped || music.getStatus() == sf::Music::Paused)
  {
    if (!paused) // resolve um pequeno bug sonoro
      music.play();
  }

  while (window.pollEvent(event))
  {
    if (event.type == sf::Event::Closed)
      window.close();

    // tratamento da janela
    if (event.type == sf::Event::Resized) // evento que deteca se o usuario redimensionou a janela
    {
      sf::FloatRect visible_area(0, 0, event.size.width, event.size.height); // cria um novo retangulo, que vai ser nossa area visivel, o tamanho é com base no tanto que o usuario redimensionou
      window.setView(sf::View(visible_area));                                // setar a visibilidade nova
    }

    // tratamento da jogabilidade
    if (event.type == event.MouseButtonPressed)
    {

      pressed = false;
    }

    // verificacao do pause
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
    {
      paused = true;
    }

    pos_mouse_win = sf::Mouse::getPosition(window);           // ficar sempre pegando a posicao do meu mouse
    pos_mouse_coord = window.mapPixelToCoords(pos_mouse_win); // converte a posicao do mouse em relacao a janela
    mouse.setPosition((sf::Vector2f)pos_mouse_win);           // a posicao da minha picareta (mouse)
  }
}
void Minecrap::runGame()
{
  try
  {
    this->verifyFolders();
  }
  catch (const char *msg)
  {
    std::cerr << "ERRO: " << msg << '\n';
    return;
  }

  while (window.isOpen())
  {
    this->loopEvents();
    if (gameover)
    {
      this->gameOver();
    }
    else
    {
      if (paused)
      {
        this->pauseGame();
      }
      else
      {
        this->codeGame();
      }
    }
  }
}