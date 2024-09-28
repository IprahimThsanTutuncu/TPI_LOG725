#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class GameOOPObject {
public:
    virtual ~GameOOPObject() = default; 
    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;

    sf::Vector2f getPosition() const { return position; }
    void setPosition(const sf::Vector2f& pos) { position = pos; }

    virtual bool hasBeenDestroyed() const = 0;

protected:
    sf::Vector2f position;
};
