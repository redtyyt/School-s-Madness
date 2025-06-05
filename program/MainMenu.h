#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class MainMenu
{
private:
    sf::Font font;
    std::vector<std::string> menuItems;
    std::vector<sf::Text> menuTexts;
public:
    MainMenu();
    void moveUp();
    void moveDown();
    void updateLayout(sf::RenderWindow &window); // nuovo metodo
    void draw(sf::RenderWindow &window);
    int selectedItem = 0;
};

