#ifndef LOADING_H
#define LOADING_H

#include <SFML/Graphics.hpp>
#include "In-Game.h"

class Loading {
public:
    Loading();
    void update(float dt);
    void draw(sf::RenderWindow &window);
private:
    sf::CircleShape throbber;
    sf::Text text;
    sf::Font font;
    sf::Texture bgTexture;        // Nuovo texture per il background
    sf::Sprite bgSprite;          // Sprite per il background
    static sf::RenderWindow locWin;
    
    float rotationSpeed; // gradi al secondo
};

#endif // LOADING_H
