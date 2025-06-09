#include "In-Game.h"
#include <iostream>
#include <thread>
#include <chrono>

/// @brief Classe che gestisce la logica di gioco e le scene di gioco.
/// InGame è responsabile del caricamento dei background, della gestione delle transizioni tra le scene e del disegno degli elementi di gioco.
InGame::InGame()
{
    loadBackground("assets/TramaInit.png"); // CARICA IL BACKGROUND INIZIALE
    getAmbienceBuf();
    playCampanaLoop();
    currentLoc = Location::Cortile; // initialize currentLoc
    targetState = Location::Cortile; // initialize targetState
    transitioning = false; // initialize transitioning

    if (!font.loadFromFile("assets/AFont.ttf")) // FILE FONT
    {
        std::cerr << "Impossibile caricare il font ingame";
        return;
    }
    

    continueText.setFont(font);                             /********************************/
    continueText.setOrigin(continueText.getOrigin());       /* SETTING DI TUTTI I PARAMETRI */
    continueText.setFillColor(sf::Color::White);            /********************************/
    continueText.setString("Clicca ENTER per continuare");
}
/// @brief Carica un'immagine di sfondo.
/// @param filename Il percorso del file da caricare.
/// @return true se il caricamento ha successo, false altrimenti.
bool InGame::loadBackground(const std::string &filename)
{
    if(!storyTexture.loadFromFile(filename))
    {
        std::cerr << "Errore nel caricamento del background della trama: " << filename << std::endl;
        return false;
    }
    storySprite.setTexture(storyTexture); // TEXTURE DELLA STORIA
    return true;
}

void InGame::playCampanaLoop()
{
    std::thread([](){ // THREAD PER IL SUONO DELLA CAMPANA
        sf::SoundBuffer buffer;
        if (!buffer.loadFromFile("assets/suoni/campana.ogg"))
        {
            std::cerr << "Impossibile caricare l'asset 'campana.ogg'" << std::endl;
            return;
        }
        sf::Sound campana(buffer);  // INSTANZA DEL SUONO
        campana.setLoop(false);     // DISATTIVA IL LOOP

        for (int i = 0; i < 12; i++) // RIPRODUCE IL SUONO 12 VOLTE (12:00 PM)
        {
            campana.play();

            while (campana.getStatus() == sf::Sound::Playing) // QUANDO SUONA FAI ASPETTARE 100ms
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            
        }
        
    }).detach(); // ESEGUI IL THREAD IN BACKGROUND
}
/// @brief Disegna gli elementi di gioco.
/// @param window La finestra di rendering.
void InGame::draw(sf::RenderWindow &window)
{
    sf::Vector2u winSize = window.getSize();

    if (sceneState == InGameState::Storia) {
        // Disegna la storia
        sf::Vector2u winSize = window.getSize();
        sf::Vector2u textureSize = storyTexture.getSize();
        float scaleX = static_cast<float>(winSize.x) / textureSize.x;
        float scaleY = static_cast<float>(winSize.y) / textureSize.y;
        storySprite.setScale(scaleX, scaleY);
        window.draw(storySprite);
        continueText.setPosition(winSize.x / 2, winSize.y * 0.8f);
        window.draw(continueText);
    }
    else if (sceneState == InGameState::Cap1Intro) {
        // Disegna l'intro capitolo
        drawCapitoloIntro(window);
        initNewCapitoloIntro(window); 
    }
    else if (sceneState == InGameState::TransitionToCortile) {
        // Fade out nero
        transitionTimer += 1.f / 60.f; // oppure usa dt
        float progress = transitionTimer / transitionDuration;
        if (progress > 1.f) progress = 1.f;
        sf::Uint8 alpha = static_cast<sf::Uint8>(255 * progress);
        transitionOverlay.setSize(sf::Vector2f(winSize.x, winSize.y));
        transitionOverlay.setFillColor(sf::Color(0, 0, 0, alpha));
        window.draw(transitionOverlay);

        if (progress >= 1.f) {
            sceneState = InGameState::Cortile;
            transitionOverlay.setFillColor(sf::Color(0, 0, 0, 0));
        }
    }
    else if (sceneState == InGameState::Cortile) {
        drawCortile(window); // Disegna il cortile
        playAmbience();
    }
}

/// @brief Crea gli sfondi per le diverse stanze.
/// @param window La finestra di rendering.
/// @return true se tutti gli sfondi sono stati caricati con successo, false altrimenti.
bool InGame::createBackgrounds(sf::RenderWindow &window)
{
    bool allLoaded = false;
    if (!cortileTexture.loadFromFile("assets/cortile.png"))
    {
        std::cerr << "File 1 not found\n";
    } else {cortile.setTexture(cortileTexture);}
    if (!scale1Texture.loadFromFile("assets/scale1.png"))
    {
        std::cerr << "File 2 not found\n";
    } else {scaleDiEntrata.setTexture(scale1Texture);}
    if (!scale2Texture.loadFromFile("assets/scale2.png"))
    {
        std::cerr << "File 3 not found\n";
    } else {scaleCentrali.setTexture(scale2Texture);}
    if (!corridoio1Texture.loadFromFile(""))
    {
        std::cerr << "File 4 not found\n";
    } else {corridoioElementari1.setTexture(corridoio1Texture);}
    if (!corridoio2Texture.loadFromFile(""))
    {
        std::cerr << "File 5 not found\n";
    } else {corridoioElementari2.setTexture(corridoio2Texture);}
    if (!corridoio3Texture.loadFromFile(""))
    {
        std::cerr << "File 6 not found\n";
    } else {corridoioElementari3.setTexture(corridoio3Texture);}
    if (!corridoio4Texture.loadFromFile(""))
    {
        std::cerr << "File 7 not found\n";
    } else {corridoioMedie1.setTexture(corridoio4Texture);}
    if (!corridoio5Texture.loadFromFile(""))
    {
        std::cerr << "File 8 not found\n";
    } else {corridoioMedie2.setTexture(corridoio5Texture);}
    
    
    allLoaded = true;

    return allLoaded;
}

void InGame::updateGameScene(sf::RenderWindow &window)
{
    static sf::Clock clock;
    float dt = clock.restart().asSeconds();

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
    switch (currentLoc)
    {
        case Location::Cortile:
            // Codice per il cortile
            break;
        case Location::ScaleDiEntrata:
            /* code */
            break;
        case Location::CorridoioElementari1:
            /* code */
            break;
        case Location::CorridoioElementari2:
            /* code */
            break;
        case Location::CorridoioElementari3:
            /* code */
            break;
        case Location::CorridoioMedie1:
            /* code */
            break;
        case Location::CorridoioMedie2:
            /* code */
            break;
        case Location::ScalePrincipali:
            /* code */
            break;
        default:
            break;
    }
}

void InGame::startTransition(Location newState, sf::RenderWindow &window)
{
    transitioning = true; // SET ISTRANSITIONING
    transitionTimer = 0.f; // RESETTA IL TIMER DI TRANSIZIONE
    targetState = newState; // SETTA IL NUOVO STATO DI DESTINAZIONE
    fadeOutPhase = false; // INIZIA CON LA FASE DI FADE IN

    if (newState == Location::Cortile)
    {
        window.draw(cortile); // DISEGNA IL CORTILE
        getAmbienceBuf();
        playAmbience();
    }
    else if (newState == Location::ScaleDiEntrata)
    {
        
    }
    else if (newState == Location::CorridoioElementari1)
    {
        /* code */
    }
    else if (newState == Location::CorridoioElementari2)
    {
        /* code */
    }
    else if (newState == Location::CorridoioElementari3)
    {
        /* code */
    }
    else if (newState == Location::CorridoioMedie1)
    {
        /* code */
    }
    else if (newState == Location::CorridoioMedie2)
    {
        /* code */
    }
    
}

void InGame::drawScene(sf::RenderWindow &window)
{
    updateGameScene(window); // AGGIORNA LA SCENA DI GIOCO
}

void InGame::initNewCapitoloIntro(sf::RenderWindow &window)
{
    // Lancia un thread per riprodurre il suono e poi avvia la transizione
    if (!introStarted)
    {
        std::thread([this, &window]{
            sf::SoundBuffer buf;
            if (!buf.loadFromFile("assets/suoni/introCapitolo.ogg"))
            {
                std::cerr << "File suono 2 non trovato" << std::endl;
                return;
            }
            sf::Sound sound;
            sound.setBuffer(buf);
            sound.setVolume(30.f);
            sound.play();

            // Attendi finché il suono non termina
            while (sound.getStatus() == sf::Sound::Playing)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            std::cout << "Sound finished" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            startTransition(Location::Cortile, window);
            std::cout << "Transition started" << std::endl;
            introStarted = true;
        }).detach();
    }
    introStarted = true;  // Impedisci ulteriori avvii
}

void InGame::drawCapitoloIntro(sf::RenderWindow &window)
{
    sf::Vector2u winSize = window.getSize();
    if (!cap1Texture.loadFromFile("assets/cap1.png"))
    {
        std::cerr << "Cap1 file not found" << std::endl;
        return;
    }
    cap1Sprite.setTexture(cap1Texture);
    sf::Vector2u textureSize = cap1Texture.getSize();
    float scaleX = static_cast<float>(winSize.x) / textureSize.x;
    float scaleY = static_cast<float>(winSize.y) / textureSize.y;
    cap1Sprite.setScale(scaleX, scaleY);

    window.draw(cap1Sprite);
    if (!introStarted)
    {
        std::thread([this, &window]{
            std::this_thread::sleep_for(std::chrono::seconds(3));
            startTransition(Location::Cortile, window);
        }).detach();
    }
}

void InGame::drawCortile(sf::RenderWindow &window)
{
    sf::Vector2u winSize = window.getSize();
    sf::Vector2u textureSize = cortileTexture.getSize();
    float scaleX = static_cast<float>(winSize.x) / textureSize.x;
    float scaleY = static_cast<float>(winSize.y) / textureSize.y;
    cortile.setScale(scaleX, scaleY);
    window.draw(cortile);
}

bool InGame::setSceneState(InGameState &newSceneState)
{
    sceneState = newSceneState;
    return true;
}

            //////////////
            // AMBIANCE //
            //////////////

void InGame::getAmbienceBuf()
{
    loaded = false;
    if (!loaded) {
        if (!ambiance.openFromFile("assets/suoni/night-ambience-normal.mp3"))
        {
            std::cerr << "Errore nel caricare il buffer dell'ambience" << std::endl;
        }
        ambiance.setVolume(40.f);
        loaded = true;
    }
}

void InGame::playAmbience()
{
    if (!play)
    {
        std::thread([this]{
            ambiance.setLoop(true);
            ambiance.play();
        }).detach();
        play = true;
    }
}

            ///////////////
            // COLLIDERS //
            ///////////////

void InGame::createColliders(const sf::RenderWindow &window)
{
    winSize = window.getSize();

    // Collider per il pavimento
    sf::RectangleShape groundCollider;
    groundCollider.setSize(sf::Vector2f(winSize.x, winSize.y * 0.1f)); // 10% dell'altezza della finestra
    groundCollider.setPosition(0.f, winSize.y * 0.9f); // Posizionato in basso
    colliders.push_back(groundCollider);

    // Collider per le scale centrali
    sf::RectangleShape stairsCollider;
    stairsCollider.setSize(sf::Vector2f(winSize.x * 0.2f, winSize.y * 0.2f)); // 20% larghezza, 20% altezza
    stairsCollider.setPosition(winSize.x * 0.4f, winSize.y * 0.7f); // Posizionato al centro
    colliders.push_back(stairsCollider);

    // Collider per il muro sinistro
    sf::RectangleShape leftWallCollider;
    leftWallCollider.setSize(sf::Vector2f(winSize.x * 0.1f, winSize.y)); // 10% larghezza, altezza completa
    leftWallCollider.setPosition(0.f, 0.f); // Posizionato a sinistra
    colliders.push_back(leftWallCollider);

    // Collider per il muro destro
    sf::RectangleShape rightWallCollider;
    rightWallCollider.setSize(sf::Vector2f(winSize.x * 0.1f, winSize.y)); // 10% larghezza, altezza completa
    rightWallCollider.setPosition(winSize.x * 0.9f, 0.f); // Posizionato a destra
    colliders.push_back(rightWallCollider);
}

void InGame::updateColliders(const sf::RenderWindow& window) 
{
    sf::Vector2u newSize = window.getSize();
    if (newSize != winSize) {
        winSize = newSize;
        colliders.clear();
        createColliders(window);
    }
}

collisionType InGame::checkCollision(const sf::FloatRect& playerBounds) 
{
    for (const auto& collider : colliders) 
    {
        if (collider.getGlobalBounds().intersects(playerBounds)) 
        {
            if (collider.getPosition().x == 0.f) return collisionType::LeftWall;
            else if (collider.getPosition().x + collider.getSize().x == winSize.x) return collisionType::RightWall;
            else if (collider.getPosition().y + collider.getSize().y == winSize.y) return collisionType::Ground;
            else if (collider.getPosition().y < playerBounds.top) return collisionType::Stairs;
        }
    }
    return collisionType::None; // Nessuna collisione
}

void InGame::drawColliders(sf::RenderWindow &window)
{
    for (const auto &collider : colliders) {
        window.draw(collider);
    }
}
