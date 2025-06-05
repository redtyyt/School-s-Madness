#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class OptionsMenu {
private:
    sf::Font font;
    sf::Texture bgTexture;        // Nuovo texture per il background
    sf::Sprite bgSprite;          // Sprite per il background
    std::vector<std::string> options = {"Audio", "Video", "Controlli", "Fullscreen", "Indietro"};
    std::vector<sf::Text> optionTexts;
    
    // Esempio di elemento interno: uno slider per il volume
    sf::RectangleShape volumeSliderBar;
    sf::CircleShape volumeSliderKnob;
public:
    int selectedOption = 0;
    OptionsMenu(sf::RenderWindow &window);
    void updateLayout(sf::RenderWindow &window);
    void moveUp();
    void moveDown();
    void draw(sf::RenderWindow &window);
};