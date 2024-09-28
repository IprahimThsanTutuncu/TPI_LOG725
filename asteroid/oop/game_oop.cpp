#include "game_oop.h"
#include <iostream>

GameOOP::GameOOP() {
    init();
}

void GameOOP::init() {
 
    spawnNewAsteroids();

    font.loadFromFile("media/comicsans.ttf");
    text.setFont(font);
    text.scale(2.0, 1.0);
    text.setPosition(100.f, 100.f);
    text.setString("Game Over!");
}
void GameOOP::update(float dt) 
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        spaceship.turn(-10.f * dt);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        spaceship.turn(10.f * dt);

    spaceship.update(dt);
    updateBullets(dt);
    updateAsteroids(dt);
    checkBulletAsteroidCollisions();
    spawnNewAsteroids();
}

void GameOOP::updateBullets(float dt) {
    for (size_t i = 0; i < bullets.size(); )
    {
        bullets[i].update(dt);
        if (bullets[i].hasBeenDestroyed()) 
            bullets.erase(bullets.begin() + i); // Remove bullet directly
        else 
            ++i; // prochain bullet
    }
}

void GameOOP::updateAsteroids(float dt) {
    for (size_t i = 0; i < asteroids.size(); i++)
    {
        asteroids[i].update(dt);

        if (spaceship.hasHitAsteroid(asteroids[i]))
        {
            spaceship.kill();
        }
    }
}

void GameOOP::checkBulletAsteroidCollisions() 
{
    for (size_t i = 0; i < bullets.size(); i++) 
    {
        bool bulletDestroyed = false;
        for (size_t j = 0; j < asteroids.size(); ) 
        {
            if (bullets[i].isCollidingWith(asteroids[j])) 
            {
                asteroids.erase(asteroids.begin() + j); // Remove asteroid directly
                bulletDestroyed = true; // Mark bullet for removal
                break;
            }
            else 
                ++j; 
        }
    }
}

void GameOOP::spawnNewAsteroids() {
    while (asteroids.size() < 50) {
        float radius = rand() % 30 + 20;
        sf::Vector2f position(
            static_cast<float>(rand() % (800 - static_cast<int>(radius * 2)) + radius),
            static_cast<float>(rand() % (600 - static_cast<int>(radius * 2)) + radius)
        );

        float spaceshipRadius = spaceship.getRadius();
        sf::Vector2f spaceshipPosition = spaceship.getPosition();

        float distance = std::hypot(position.x - spaceshipPosition.x, position.y - spaceshipPosition.y);

        if (distance > (radius + spaceshipRadius)) 
        {
            asteroids.emplace_back(radius, position);
        }
    }
}



void GameOOP::draw(sf::RenderWindow& window) {
    window.clear();

    spaceship.draw(window); 

    for (const auto& bullet : bullets) {
        bullet.draw(window);
    }

    for (const auto& asteroid : asteroids) {
        asteroid.draw(window);
    }

    if (spaceship.hasBeenDestroyed())
    {
        window.draw(text);
    }
    window.display();
}

void GameOOP::input(sf::Event event) 
{
    if (event.type == sf::Event::Closed) {
        isRunning = false;
    }

    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Space)
        {
            Bullet newBullet(10.f, spaceship.getDirection(), spaceship.getPosition());
            bullets.push_back(newBullet);
        }
    }
}
