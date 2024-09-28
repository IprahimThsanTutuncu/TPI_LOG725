#pragma once

#include <SFML/Graphics/CircleShape.hpp>
#include "game_object.h"

class Asteroid;

class Bullet : public GameOOPObject {
public:
    Bullet(float speed, sf::Vector2f direction, sf::Vector2f position);

    bool isCollidingWith(const Asteroid& asteroid);

    void update(float dt) override;
    void draw(sf::RenderWindow& window) const override;

    bool hasBeenDestroyed() const override;
    void kill() { ImDead = true; }

private:
    float lifeTime{ 60.f };
    bool ImDead{ false };
    float radius{ 5.f };
    float speed{ 20.f };
    sf::Vector2f direction;
    sf::CircleShape shape;
};
