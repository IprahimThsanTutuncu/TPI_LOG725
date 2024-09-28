#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "spaceship.h"
#include "asteroid.h" 
#include "bullet.h"   

class GameOOP {
public:
    GameOOP();

    void init();
    void update(float dt);
    void draw(sf::RenderWindow& window);
    void input(sf::Event event);

    bool isRunning{ false };
private:
    void updateBullets(float dt);
    void updateAsteroids(float dt);
    void checkBulletAsteroidCollisions();
    void spawnNewAsteroids();

    sf::Font font;
    sf::Text text;
    Spaceship spaceship; 
    std::vector<Asteroid> asteroids;
    std::vector<Bullet> bullets;
};
