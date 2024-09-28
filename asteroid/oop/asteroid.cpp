#include "Asteroid.h"
#include <SFML/Graphics/Color.hpp>
#include <cmath>
#include <random>

const float M_PI = 3.14159;

Asteroid::Asteroid(float radius, sf::Vector2f position)
    : isDestroyed(false), speed(std::max(1.f, 10.f / radius)), rayon(radius)
{
    this->position = position;
    shape.setRadius(radius);
    shape.setPosition(position);
    shape.setOrigin(radius, radius);

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> colorDist(100, 200);
    sf::Color color(colorDist(rng), colorDist(rng), colorDist(rng));
    shape.setFillColor(color);
}

void Asteroid::update(float dt) {
    if (!isDestroyed) {
        position += direction * speed * dt;

        direction += sf::Vector2f((std::rand() % 100 - 50) / 1000.f, (std::rand() % 100 - 50) / 1000.f);

        // Normalize direction
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length > 0) {
            direction /= length;
        }

        shape.setPosition(position); 
    }
}

bool Asteroid::hasBeenDestroyed() const {
    return isDestroyed;
}

void Asteroid::draw(sf::RenderWindow& window) const {
    if (!isDestroyed) {
        window.draw(shape);
    }
}

float Asteroid::getRadius() const {
    return rayon;
}

sf::Vector2f Asteroid::randomPosition() {
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(0, 800);
    return sf::Vector2f(dist(rng), dist(rng));
}
