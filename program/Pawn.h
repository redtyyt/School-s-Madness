#pragma once

#include <SFML/Graphics.hpp>

class Pawn
{
private:
    sf::RectangleShape shape; // Forma del pedone
    float velocity; // Velocità del pedone
    float velocityY;
    float gravity;


public:
    Pawn(float size = 50.f, float speed = 100.f, float gravity = 9.81f);
    void move(const sf::Vector2f& direction, float deltaTime);
    void applyGravity(float dt);
    void draw(sf::RenderWindow& window);
    bool setVelY(float newVelocityY);
    sf::Vector2f getPosition() const;
    void setPosition(const sf::Vector2f& position);
    sf::FloatRect getBounds() const;
    void setDefaultPos(sf::RenderWindow& window);
    void handleCollisionWithGround(Pawn& player, const sf::RenderWindow& window);
    ~Pawn();
};