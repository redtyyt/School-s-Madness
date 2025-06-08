#include "Loading.h"
#include <iostream>

Loading::Loading() : rotationSpeed(180.f) 
{
    if (!bgTexture.loadFromFile("assets/optionsBackground.png"))
    {
        std::cout << "Immagine di background non trovata" << std::endl;
    }
    bgSprite.setTexture(bgTexture);
    // Puoi scalare il background se necessario (ad esempio, adattandolo alle dimensioni della finestra

    if (!font.loadFromFile("assets/AFont.ttf"))
    {
        std::cout << "Font non trovato" << std::endl;
    }
    throbber.setRadius(30.f);
    throbber.setPointCount(10);  // usa meno punti per creare un gap visibile
    throbber.setOutlineThickness(5.f);
    throbber.setOutlineColor(sf::Color::White); // colore contrastante
    throbber.setFillColor(sf::Color::Transparent);
    throbber.setOrigin(throbber.getRadius(), throbber.getRadius());

    text.setFont(font);
    text.setOrigin(text.getOrigin());
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(50);
    text.setString("Caricamento...");
    text.setPosition(1280 / 3 - 100, 720 / 3);
}

void Loading::update(float dt) {
    throbber.rotate(rotationSpeed * dt);
}

void Loading::draw(sf::RenderWindow &window) {
    /*
        @return void
        - Renderizza la finestra di loading
    */
    // Ottieni la dimensione attuale della finestra
    sf::Vector2u winSize = window.getSize();
    
    // InGame *game = new InGame();
    // game->createBackgrounds(window);
    // delete game;
    
    // Aggiorna lo scale del background di loading in base alla finestra
    bgSprite.setScale(
        static_cast<float>(winSize.x) / bgSprite.getLocalBounds().width,
        static_cast<float>(winSize.y) / bgSprite.getLocalBounds().height
    );
    bgSprite.setPosition(0.f, 0.f);

    throbber.setPosition(winSize.x / 2.f, winSize.y / 2.f);
    text.setPosition(winSize.x / 2.f - text.getLocalBounds().width / 2.f, winSize.y / 3.f);
    
    // Disegna il background di loading, il throbber e il testo
    window.draw(bgSprite);
    window.draw(throbber);
    window.draw(text);
}
