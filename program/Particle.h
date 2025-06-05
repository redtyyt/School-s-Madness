#pragma once
#include <SFML/Graphics.hpp>
#include <cstdlib>  // per rand()

class Particle {
public:
    sf::CircleShape shape;
    sf::Vector2f velocity;

    Particle(float radius, const sf::Vector2f &position, const sf::Vector2f &vel)
        : velocity(vel)
    {
        shape.setRadius(radius);
        shape.setOrigin(radius, radius); // centra la particella
        shape.setPosition(position);
        // Genera colore casuale tra quasi bianco e grigio
        int base = 200;  // valore base (grigio chiaro)
        int range = 56;  // 200 + (0..55) => [200,255]
        int r = base + (rand() % range);
        int g = base + (rand() % range);
        int b = base + (rand() % range);
        shape.setFillColor(sf::Color(r, g, b));
    }

    void update(float dt) {
        shape.move(velocity * dt);
    }
};
