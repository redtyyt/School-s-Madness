#include "OptionsMenu.h"
#include <iostream>

OptionsMenu::OptionsMenu(sf::RenderWindow &window) {
    if (!font.loadFromFile("assets/AFont.ttf"))
        std::cout << "Impossibile trovare il font per le opzioni" << std::endl;

    if (!bgTexture.loadFromFile("assets/optionsBackground.png"))
        std::cout << "Impossibile caricare il background delle opzioni" << std::endl;
    bgSprite.setTexture(bgTexture);
    // Scale iniziale (verrà aggiornato in updateLayout)
    bgSprite.setScale(1280.f / bgSprite.getLocalBounds().width, 720.f / bgSprite.getLocalBounds().height);

    for (size_t i = 0; i < options.size(); i++) {
        sf::Text text;
        text.setFont(font);
        text.setString(options[i]);
        text.setCharacterSize(40);
        // Posizione iniziale (da aggiornare)
        float x = 1280 / 2 - 100;
        float y = 200 + i * 50;
        text.setPosition(x, y);
        text.setFillColor(i == selectedOption ? sf::Color::Red : sf::Color::White);
        optionTexts.push_back(text);
    }
    
    // Inizializzazione slider (posizione iniziale fissa, aggiornabile in updateLayout)
    volumeSliderBar.setSize(sf::Vector2f(200.f, 10.f));
    volumeSliderBar.setFillColor(sf::Color(180,180,180));
    volumeSliderBar.setPosition(1280 / 2 - 100, 400);

    volumeSliderKnob.setRadius(8.f);
    volumeSliderKnob.setFillColor(sf::Color::Red);
    volumeSliderKnob.setOrigin(volumeSliderKnob.getRadius(), volumeSliderKnob.getRadius());
    volumeSliderKnob.setPosition(volumeSliderBar.getPosition().x + volumeSliderBar.getSize().x * 0.5f,
                                   volumeSliderBar.getPosition().y + volumeSliderBar.getSize().y / 2);
}

void OptionsMenu::updateLayout(sf::RenderWindow &window)
{
    sf::Vector2u winSize = window.getSize();
    // Aggiorna lo scale dello sfondo per coprire l'intera finestra
    bgSprite.setScale(
        static_cast<float>(winSize.x) / bgSprite.getLocalBounds().width,
        static_cast<float>(winSize.y) / bgSprite.getLocalBounds().height
    );

    // Ricalcola la posizione dei testi
    for (size_t i = 0; i < optionTexts.size(); i++) {
        sf::Text &text = optionTexts[i];
        float x = winSize.x / 2.f - text.getLocalBounds().width / 2.f;
        float y = winSize.y * 0.3f + i * (text.getCharacterSize() + 15);
        text.setPosition(x, y);
    }

    // Posiziona lo slider al centro
    volumeSliderBar.setPosition(winSize.x / 2.f - volumeSliderBar.getSize().x / 2.f, winSize.y * 0.6f);
    volumeSliderKnob.setPosition(volumeSliderBar.getPosition().x + volumeSliderBar.getSize().x * 0.5f,
                                   volumeSliderBar.getPosition().y + volumeSliderBar.getSize().y / 2.f);
}

void OptionsMenu::draw(sf::RenderWindow &window) {
    updateLayout(window);
    window.draw(bgSprite);
    for (auto &text : optionTexts)
        window.draw(text);
    window.draw(volumeSliderBar);
    window.draw(volumeSliderKnob);
}

void OptionsMenu::moveUp() {
    if(selectedOption > 0) {
        optionTexts[selectedOption].setFillColor(sf::Color::White);
        selectedOption--;
        optionTexts[selectedOption].setFillColor(sf::Color::Red);
    }
}

void OptionsMenu::moveDown() {
    if(selectedOption < optionTexts.size() - 1) {
        optionTexts[selectedOption].setFillColor(sf::Color::White);
        selectedOption++;
        optionTexts[selectedOption].setFillColor(sf::Color::Red);
    }
}