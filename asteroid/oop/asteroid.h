#pragma once

#include <SFML/Graphics/VertexArray.hpp>
#include "game_object.h"
#include <SFML/Graphics/CircleShape.hpp>

class Asteroid : public GameOOPObject {
public:
    Asteroid(float radius, sf::Vector2f position);

    void update(float dt) override;

    bool hasBeenDestroyed() const override;
    void draw(sf::RenderWindow& window) const override;

    float getRadius() const;

private:
    bool isDestroyed;
    sf::CircleShape shape;
    sf::Vector2f direction;
    float speed;
    float rayon;

    sf::Vector2f randomPosition();
};
