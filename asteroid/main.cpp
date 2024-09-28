#include <SFML/Graphics.hpp>
#include "oop/game_oop.h"
#include "ecs/game_ecs.h"
int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Asteroid GameOOP");

    //changer l'initialisation pour changer
    //la version utiliser

    GameOOP game; 
    //GameECS game;

    game.init();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed)
                window.close();

            game.input(event); 
        }

        float dt = 1.f / 60.f;
        game.update(dt); 
        game.draw(window);
    }

    return 0;
}
