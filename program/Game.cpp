#include "Game.h"
#include "MainMenu.h"
#include "Loading.h"
#include <cstdlib>  // per rand()

void Game::initializeVars()
{
    this->window = nullptr;
}

void Game::initWindow()
{
    if(fullscreen)
    {
        // Ottieni la risoluzione attuale del desktop
        videoMode = sf::VideoMode::getDesktopMode();
        this->window = new sf::RenderWindow(videoMode, "Game", sf::Style::Fullscreen);
    }
    else
    {
        videoMode.height = 720;
        videoMode.width = 1280;
        this->window = new sf::RenderWindow(videoMode, "Game", sf::Style::Titlebar | sf::Style::Close);
    }
    
    playMenuBackgroundMusic();
    // Aggiorna la dimensione dell'overlay della transizione
    transitionOverlay.setSize(sf::Vector2f(videoMode.width, videoMode.height));
    transitionOverlay.setFillColor(sf::Color(0, 0, 0, 0));
}

void Game::initParticles()
{
    // Crea 50 particelle
    for (int i = 0; i < 50; i++) {
        float radius = 1.f + static_cast<float>(rand() % 3);  // raggio variabile tra 1 e 3
        sf::Vector2f position(rand() % videoMode.width, rand() % videoMode.height);
        // Velocità casuale: valori fra -50 e 50 (pixel al secondo)
        float velX = (rand() % 101 - 50) / 1.f;
        float velY = (rand() % 101 - 50) / 1.f;
        Particle particle(radius, position, sf::Vector2f(velX, velY));
        particle.shape.setFillColor(sf::Color(255, 255, 255, 127)); // Set transparency to 0.5
        particles.push_back(particle);
    }
}

Game::Game()
{
    this->initializeVars();
    this->initWindow();
    // Crea l'istanza di OptionsMenu passando la finestra esistente
    this->optionsMenu = new OptionsMenu(*this->window);
    this->initParticles();
}

Game::~Game()
{
    delete this->window;
    delete this->optionsMenu;
    if (gameScene != nullptr)
    {
        delete gameScene;
    }
    
}

const bool Game::running() const
{
    return this->window->isOpen();
}

void Game::drawText(sf::Text text)
{
    this->window->draw(text);
}

sf::Sprite Game::getBackground()
{
    if (!bgTexture.loadFromFile("assets/mainMenuNoText.png"))
    {
        std::cout << "Immagine background non trovata." << std::endl;
    }
    sf::Sprite background(bgTexture);
    
    // Calcola i fattori di scala per adattare l'immagine alla finestra
    float scaleX = static_cast<float>(videoMode.width) / background.getLocalBounds().width;
    float scaleY = static_cast<float>(videoMode.height) / background.getLocalBounds().height;
    background.setScale(scaleX, scaleY);
    
    return background;
}

void Game::startTransition(GameState newState)
{
    transitioning = true;
    transitionTimer = 0.f;
    targetState = newState;
}

void Game::toggleFullscreen() {
    // Alterna il flag
    fullscreen = !fullscreen;
    
    // Chiudi e distruggi la finestra corrente
    if(this->window) {
        this->window->close();
        delete this->window;
    }
    
    // Ricrea la finestra con il nuovo stile fullscreen/finestretta
    this->initWindow();
    
    // Poiché OptionsMenu è stato creato passandogli il puntatore alla finestra,
    // dobbiamo ricrearlo per usare la nuova finestra:
    if(optionsMenu) {
        delete optionsMenu;
    }
    optionsMenu = new OptionsMenu(*this->window);
}

void Game::playMenuBackgroundMusic()
{
    if (currentState == GameState::MainMenu || currentState == GameState::Options)
    {
        if (!backMenuMusic.openFromFile("assets/musica/Main Menu.wav"))
        {
            std::cout << "Impossibile trovare la canzone di background" << std::endl;
        }
        backMenuMusic.setLoop(true);
        backMenuMusic.setVolume(30.f);
        backMenuMusic.play();
    }
}

void Game::stopMenuBackgroundMusic()
{
    backMenuMusic.stop();
}

void Game::playButtonSound()
{
    static sf::SoundBuffer buffer1;
    if (!buffer1.loadFromFile("assets/suoni/pulsante.ogg"))
    {
        std::cerr << "Impossibile trovare il file .ogg di un suono";
        return;
    }
    
    static sf::Sound buttonClicked(buffer1);
    buttonClicked.setVolume(30);
    buttonClicked.setLoop(false);
    buttonClicked.play();
}

void Game::pollEvents()
{
    while (this->window->pollEvent(this->ev))
    {
        switch (this->ev.type)
        {
            case sf::Event::Closed:
                this->window->close();
                break;
            case sf::Event::KeyPressed:
                if(ev.key.code == sf::Keyboard::Escape)
                {
                    // Se sei nella schermata delle opzioni, torna al menu principale
                    if(currentState == GameState::Options)
                        currentState = GameState::MainMenu;
                    else
                        this->window->close();
                }
                else if (ev.key.code == sf::Keyboard::Up)
                {
                    if(currentState == GameState::MainMenu)
                        menu.moveUp();
                    else if(currentState == GameState::Options)
                        optionsMenu->moveUp();
                }
                else if (ev.key.code == sf::Keyboard::Down)
                {
                    if(currentState == GameState::MainMenu)
                        menu.moveDown();
                    else if(currentState == GameState::Options)
                        optionsMenu->moveDown();
                }
                else if (ev.key.code == sf::Keyboard::Enter)
                {
                    if(currentState == GameState::MainMenu)
                    {
                        if (menu.selectedItem == 0)
                        {
                            std::cout << "Gioca" << std::endl;
                            playButtonSound();
                            stopMenuBackgroundMusic();
                            startTransition(GameState::Loading);
                        }
                        else if (menu.selectedItem == 1)
                        {
                            std::cout << "Opzioni" << std::endl;
                            playButtonSound();
                            startTransition(GameState::Options);
                        }
                        else if (menu.selectedItem == 2)
                        {
                            std::cout << "Esci" << std::endl;
                            playButtonSound();
                            this->window->close();
                        }
                    }
                    else if(currentState == GameState::Options)
                    {
                        // Selezione nelle opzioni:
                        // Supponiamo: 
                        // Indice 3 -> toggla fullscreen
                        // Indice 4 -> torna al MainMenu
                        if(optionsMenu->selectedOption == 3) {
                            playButtonSound();
                            toggleFullscreen();
                        }
                        else if(optionsMenu->selectedOption == 4) {
                            playButtonSound();
                            currentState = GameState::MainMenu;
                        }
                    }
                    else if (currentState == GameState::Play)
                    {
                        playButtonSound();
                        startTransition(GameState::Game);
                    }
                }
                break;
        }
    }
}

void Game::updateParticles(float dt)
{
    for (auto &particle : particles) {
        particle.update(dt);
        // Se la particella esce dallo schermo, la ricolloca dall'altro lato
        sf::Vector2f pos = particle.shape.getPosition();
        if (pos.x < 0) pos.x = videoMode.width;
        if (pos.x > videoMode.width) pos.x = 0;
        if (pos.y < 0) pos.y = videoMode.height;
        if (pos.y > videoMode.height) pos.y = 0;
        particle.shape.setPosition(pos);
    }
}

void Game::update()
{
    static sf::Clock clock;
    float dt = clock.restart().asSeconds();
    this->dt = dt;

    this->pollEvents();

    // Aggiorna le particelle solo se siamo nel MainMenu e non in transizione
    if (currentState == GameState::MainMenu && !transitioning)
    {
        this->updateParticles(dt);
    }

    // Gestione della transizione a due fasi
    if (transitioning)
    {
        transitionTimer += dt;

        if (!fadeOutPhase)
        {
            // Fase di fade in: aumentiamo l'alpha da 0 a 255
            float progress = transitionTimer / transitionDuration;
            if (progress > 1.f)
                progress = 1.f;
            sf::Uint8 alpha = static_cast<sf::Uint8>(255 * progress);
            transitionOverlay.setFillColor(sf::Color(0, 0, 0, alpha));

            // Se abbiamo completato il fade in, cambia lo stato e passa al fade out
            if (transitionTimer >= transitionDuration)
            {
                currentState = targetState;
                fadeOutPhase = true;
                transitionTimer = 0.f;
            }
        }
        else
        {
            // Fase di fade out: diminuisci l'alpha da 255 a 0
            float progress = transitionTimer / transitionDuration;
            if (progress > 1.f)
                progress = 1.f;
            sf::Uint8 alpha = static_cast<sf::Uint8>(255 * (1 - progress));
            transitionOverlay.setFillColor(sf::Color(0, 0, 0, alpha));

            if (transitionTimer >= transitionDuration)
            {
                // Terminata la transizione: reset
                transitioning = false;
                fadeOutPhase = false;
                transitionTimer = 0.f;
                transitionOverlay.setFillColor(sf::Color(0, 0, 0, 0));
            }
        }
    }
}

void Game::render()
{
    this->window->clear();

    switch(currentState)
    {
        case GameState::MainMenu:
            {
                sf::Sprite background = getBackground();
                this->window->draw(background);
                menu.draw(*this->window);
                for (auto &particle : particles) {
                    this->window->draw(particle.shape);
                }
            }
            break;
        case GameState::Options:
            {
                sf::Sprite background = getBackground();
                this->window->draw(background);
                optionsMenu->draw(*this->window);
            }
            break;
        case GameState::Play:
            if (gameScene == nullptr)
            {
                gameScene = new InGame();
            }
            gameScene->draw(*this->window);
            break;
        case GameState::Loading:
            loading.update(this->dt);
            loading.draw(*this->window);
            loadingStateTimer += this->dt;
            if(loadingStateTimer >= loadingDelay)
            {
                if (gameScene == nullptr)
                {
                    gameScene = new InGame();
                    if (!gameScene->createBackgrounds(*window))
                    {
                        std::cerr << "Errore nel caricamento dei background di gioco";
                    }
                    
                }
                
                 startTransition(GameState::Play);
                 loadingStateTimer = 0.f;
            }
            break;
        case GameState::Game:
            gameScene->drawScene(*this->window);
            break;
    }

    // Disegna l'overlay di transizione, se attivo
    if(transitioning)
        this->window->draw(transitionOverlay);

    this->window->display();
    std::cout << "Alpha overlay: " << (int)transitionOverlay.getFillColor().a << std::endl;
}
