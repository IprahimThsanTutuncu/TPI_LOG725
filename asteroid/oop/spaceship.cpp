#include "Spaceship.h"
#include "Bullet.h"
#include "Asteroid.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <cmath>

const float M_PI = 3.14159;

Spaceship::Spaceship() {

    ship.setPointCount(3); 
    ship.setPoint(0, sf::Vector2f(0.f, -20.f)); 
    ship.setPoint(1, sf::Vector2f(-10.f, 10.f));
    ship.setPoint(2, sf::Vector2f(10.f, 10.f)); 

    ship.setFillColor(sf::Color::Red); 
    direction = sf::Vector2f(0.f, -1.f);

    ship.setPosition(400, 300);
    position = ship.getPosition();
}

void Spaceship::update(float deltaTime)
{
    sf::Vector2f movement = direction * speed * deltaTime;
    ship.move(movement);

    position = ship.getPosition();
}

void Spaceship::draw(sf::RenderWindow& window) const {
    window.draw(ship);
}

void Spaceship::turn(float angle) {
    ship.rotate(angle);

    float radians = angle * (M_PI / 180.f);
    float cosAngle = std::cos(radians);
    float sinAngle = std::sin(radians);
    sf::Vector2f newDirection = sf::Vector2f(direction.x * cosAngle - direction.y * sinAngle,
        direction.x * sinAngle + direction.y * cosAngle);
    direction = newDirection;
}

Bullet* Spaceship::shoot()
{
    return  new Bullet(5.f, direction, ship.getPosition());
}

bool Spaceship::hasHitAsteroid(const Asteroid& asteroid) const
{
    float distance = std::hypot(ship.getPosition().x - asteroid.getPosition().x, ship.getPosition().y - asteroid.getPosition().y);
    return distance < (20.f + asteroid.getRadius());
}
