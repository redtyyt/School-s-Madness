#include "MainMenu.h"
#include <iostream>

#include <SFML/Graphics.hpp>

MainMenu::MainMenu()
{
    if (!font.loadFromFile("D:\\Users\\RedTy_ YT\\Desktop\\Scuola\\LevelsSchoolMadness\\program\\AFont.ttf"))
    {
        std::cout << "Impossibile trovare il font" << std::endl;
    }

    menuItems = {"Gioca", "Opzioni", "Esci"};

    for (size_t i = 0; i < menuItems.size(); i++)
    {
        sf::Text text;
        text.setFont(font);
        text.setString(menuItems[i]);
        text.setCharacterSize(50);
        text.setFillColor(i == selectedItem ? sf::Color::Red : sf::Color::White);
        menuTexts.push_back(text);
    }
}

void MainMenu::updateLayout(sf::RenderWindow &window)
{
    sf::Vector2u winSize = window.getSize();
    for (size_t i = 0; i < menuTexts.size(); i++)
    {
        sf::Text &text = menuTexts[i];
        float x = winSize.x / 4.f - text.getLocalBounds().width / 2.f;
        float y = winSize.y * 0.6f + i * (text.getCharacterSize() + 20);
        text.setPosition(x, y);
    }
}

void MainMenu::moveUp()
{
    if (selectedItem > 0) {
        menuTexts[selectedItem].setFillColor(sf::Color::White);
        selectedItem--;
        menuTexts[selectedItem].setFillColor(sf::Color::Red);
    }
}

void MainMenu::moveDown()
{
    if (selectedItem < menuTexts.size() - 1) {
        menuTexts[selectedItem].setFillColor(sf::Color::White);
        selectedItem++;
        menuTexts[selectedItem].setFillColor(sf::Color::Red);
    }
}

void MainMenu::draw(sf::RenderWindow &window)
{
    updateLayout(window);
    for (auto &text : menuTexts)
    {
        window.draw(text);
    }
}
