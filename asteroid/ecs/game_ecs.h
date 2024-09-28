#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "registry.h"
#include "system.h"

class GameECS {
public:
    GameECS();

    void init();
    void update(float dt);
    void draw(sf::RenderWindow& window);
    void input(sf::Event event);

    bool isRunning{ false };
private:
    Registry registry;

    SystemInput systemInput;
    SystemMove systemMove;
    SystemCollide systemCollide;
    SystemUpdate systemUpdate;
    SystemRender systemRender;
    SystemEvent systemEvent;
};
