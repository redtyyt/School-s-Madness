#include "In-Game.h"
#include <iostream>
#include <thread>
#include <chrono>

InGame::InGame()
{
    loadBackground("D:\\Users\\RedTy_ YT\\Desktop\\Scuola\\LevelsSchoolMadness\\program\\TramaInit.png");
    playCampanaLoop();
    currentLoc = Location::Cortile; // initialize currentLoc

    if (!font.loadFromFile("D:\\Users\\RedTy_ YT\\Desktop\\Scuola\\LevelsSchoolMadness\\program\\AFont.ttf"))
    {
        std::cerr << "Impossibile caricare il font ingame";
        return;
    }
    

    continueText.setFont(font);
    continueText.setOrigin(continueText.getOrigin());
    continueText.setFillColor(sf::Color::White);
    continueText.setString("Clicca ENTER per continuare");
}

bool InGame::loadBackground(const std::string &filename)
{
    if(!storyTexture.loadFromFile(filename))
    {
        std::cerr << "Errore nel caricamento del background della trama: " << filename << std::endl;
        return false;
    }
    storySprite.setTexture(storyTexture);
    return true;
}

void InGame::playCampanaLoop()
{
    std::thread([](){
        sf::SoundBuffer buffer;
        if (!buffer.loadFromFile("D:\\Users\\RedTy_ YT\\Desktop\\Scuola\\LevelsSchoolMadness\\program\\suoni\\campana.ogg"))
        {
            std::cerr << "Impossibile caricare l'asset 'campana.ogg'" << std::endl;
            return;
        }
        sf::Sound campana(buffer);
        campana.setLoop(false);

        for (int i = 0; i < 12; i++)
        {
            campana.play();

            while (campana.getStatus() == sf::Sound::Playing)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            
        }
        
    }).detach();
}

void InGame::draw(sf::RenderWindow &window)
{
    sf::Vector2u winSize = window.getSize();
    sf::Vector2u textureSize = storyTexture.getSize();
    float scaleX = static_cast<float>(winSize.x) / textureSize.x;
    float scaleY = static_cast<float>(winSize.y) / textureSize.y;
    storySprite.setScale(scaleX, scaleY);
    window.draw(storySprite);
    continueText.setPosition(winSize.x / 3 + winSize.x / 4, winSize.y / 3 + winSize.y / 2);
    continueText.setCharacterSize(winSize.x / 40);
    window.draw(continueText);
}

bool InGame::createBackgrounds(sf::RenderWindow &window)
{
    bool allLoaded = false;
    if (!cortileTexture.loadFromFile(""))
    {
        std::cerr << "File 1 not found\n";
    } else {cortile.setTexture(cortileTexture);}
    if (!scale1Texture.loadFromFile(""))
    {
        std::cerr << "File 2 not found\n";
    } else {scaleDiEntrata.setTexture(scale1Texture);}
    if (!scale2Texture.loadFromFile(""))
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

void InGame::updateGameScene()
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

void InGame::startTransition(Location newState)
{
    transitioning = true;
    transitionTimer = 0.f;
    targetState = newState;
}

void InGame::drawScene(sf::RenderWindow &window)
{
    sf::Vector2u winSize = window.getSize();
    updateGameScene();
    initNewCapitoloIntro(window);
}

void InGame::initNewCapitoloIntro(sf::RenderWindow &window)
{
    drawCapitoloIntro(window);
    // Lancia un thread per riprodurre il suono (solo suono, non operazioni di disegno!)
    if (!introStarted)
    {
        std::thread([this]{
            sf::SoundBuffer buf;
            if (!buf.loadFromFile("D:\\Users\\RedTy_ YT\\Desktop\\Scuola\\LevelsSchoolMadness\\program\\suoni\\introCapitolo.ogg"))
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
            introStarted = true;
        }).detach();
    }
    introStarted = true;  // Impedisci ulteriori avvii
}

void InGame::drawCapitoloIntro(sf::RenderWindow &window)
{
    sf::Vector2u winSize = window.getSize();
    if (!cap1Texture.loadFromFile("D:\\Users\\RedTy_ YT\\Desktop\\Scuola\\LevelsSchoolMadness\\program\\cap1.png"))
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
        std::thread([this]{
            std::this_thread::sleep_for(std::chrono::seconds(3));
            startTransition(Location::Cortile);
        }).detach();
    }
}

void InGame::drawCortile(sf::RenderWindow &window)
{
    sf::Vector2u winSize = window.getSize();
}
