#pragma once

#include <SFML/Graphics/ConvexShape.hpp>
#include "game_object.h"

class Bullet;
class Asteroid;

class Spaceship : public GameOOPObject {
public:
    Spaceship();

    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) const override;

    void turn(float angle);
    Bullet* shoot();
    sf::Vector2f getDirection() { return direction; }
    bool hasHitAsteroid(const Asteroid& asteroid) const; // this is game-over, not gonna lie
    void kill() { isDestroyed = true; }
    float getRadius() { return ship.getGlobalBounds().height > ship.getGlobalBounds().width ? ship.getGlobalBounds().height : ship.getGlobalBounds().width; }

    bool hasBeenDestroyed() const override { return isDestroyed; }


private:

    sf::ConvexShape ship;
    sf::Vector2f direction;
    float speed{ 2.f };
    bool isDestroyed{ false };
};
