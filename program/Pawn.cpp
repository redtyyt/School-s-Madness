#include "Pawn.h"
#include <iostream>

Pawn::Pawn(float size, float speed, float gravity) : velocity(speed), velocityY(0.f), gravity(gravity)
{
    shape.setSize(sf::Vector2f(size, size));
    shape.setFillColor(sf::Color::Green); // Colore del pedone
    shape.setOrigin(size / 2, size / 2); // Imposta l'origine
}

void Pawn::move(const sf::Vector2f& direction, float deltaTime)
{
    shape.move(direction * velocity * deltaTime);
}

void Pawn::applyGravity(float dt)
{
    if (velocityY == 0.f)
    {
        return;
    }
    
    velocityY += gravity * dt * 10;
    shape.move(0.f, velocityY * dt);
}

void Pawn::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

bool Pawn::setVelY(float newVelocityY)
{
    velocityY = newVelocityY;
    return true;
}

sf::Vector2f Pawn::getPosition() const
{
    return shape.getPosition();
}

void Pawn::setPosition(const sf::Vector2f& position)
{
    shape.setPosition(position);
    std::cout << "Player position updated: " << position.x << ", " << position.y << std::endl;
}

sf::FloatRect Pawn::getBounds() const
{
    return shape.getGlobalBounds();
}

void Pawn::setDefaultPos(sf::RenderWindow &window)
{
    sf::Vector2u winSize = window.getSize();

    shape.setPosition(10.f, winSize.y - 600.f);
}

Pawn::~Pawn()
{
    //////////////////
    // Nothing here //
    //////////////////
}

void Pawn::handleCollisionWithGround(Pawn& player, const sf::RenderWindow& window)
{
    sf::Vector2f plPos = player.getPosition();
    float groundY = window.getSize().y * 0.9f; // Posizione del pavimento
    float playerHeight = player.getBounds().height;   // Altezza del player

    // Posiziona il player sopra il pavimento
    player.setPosition(sf::Vector2f(plPos.x, groundY - playerHeight));
    player.setVelY(0.f); // Resetta la velocità verticale

    // Evita che la gravità venga applicata
    std::cout << "Player collision with ground. Position updated to: " 
              << player.getPosition().x << ", " << player.getPosition().y << std::endl;
    return;
}
