#include "bullet.h"
#include "asteroid.h"
#include <SFML/Graphics/RenderWindow.hpp>

Bullet::Bullet(float speed, sf::Vector2f direction, sf::Vector2f position)
    : speed(speed), direction(direction)
{
    this->position = position;
    shape.setRadius(radius);
    shape.setFillColor(sf::Color::Yellow);
}

bool Bullet::isCollidingWith(const Asteroid& asteroid)
{
    float distance = std::hypot(position.x - asteroid.getPosition().x, position.y - asteroid.getPosition().y);
    return distance < (shape.getRadius() + asteroid.getRadius());
}

void Bullet::update(float dt) 
{
    lifeTime -= dt;
    position += direction * speed * dt; 
    shape.setPosition(position);

    if (lifeTime < 0)
        kill();
}

void Bullet::draw(sf::RenderWindow& window) const
{
    window.draw(shape);
}

bool Bullet::hasBeenDestroyed() const
{
    return ImDead;
}
