#pragma once
#include "registry.h"
#include <variant>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

template<typename T>
concept System = requires(T t, Registry reg, float dt) {
    { t.setRegistry(reg) } -> std::same_as<void>;
};

class SystemInput
{
public:
    void setRegistry(Registry& registry) 
    {
        this->pRegistry = &registry;

        auto compPlayer = registry.getAll<PlayerComp>();
        if (compPlayer.size() > 0)
        {
            player = registry.getEntity(compPlayer[0].entityID);
        }
    };
    void update(float dt);
private:
    Entity player;
    Registry* pRegistry;
};

class SystemMove
{
public:
    void setRegistry(Registry& registry)
    {
        this->pRegistry = &registry;
    }
    void update(float dt);
private:
    Registry* pRegistry;
};


class SystemCollide
{
public:
    void setRegistry(Registry& registry)
    {
        this->pRegistry = &registry;
    }
    void update(float dt);
private:
    Registry* pRegistry;
};

class SystemUpdate
{
public:
    void setRegistry(Registry& registry)
    {
        this->pRegistry = &registry;
    }
    void update(float dt);
private:
    Registry* pRegistry;
};


class SystemRender
{
public:
    SystemRender()
    {
        circleTexture.loadFromFile("media/cercle.png");
        circle.setTexture(circleTexture);

        font.loadFromFile("media/comicsans.ttf");
        text.setFont(font);
        text.scale(2.0, 1.0);
        text.setPosition(100.f, 100.f);
        text.setString("Game Over!");

        triangle.setPointCount(3);
        triangle.setPoint(0, sf::Vector2f(0.f, -20.f));
        triangle.setPoint(1, sf::Vector2f(-10.f, 10.f));
        triangle.setPoint(2, sf::Vector2f(10.f, 10.f));


    }
    void setRegistry(Registry& registry)
    {
        this->pRegistry = &registry;
    }
    void update(float dt, sf::RenderWindow& window);
private:
    Registry* pRegistry;
    sf::Color color;
    //you may be wondering, pourquoi je n'utilise pas circleShape ici?
    //pourtant gameOOP le fait. Enfaite, gameOOP ne resize pas le même cercle
    //il aurant N nombre de circleShape qui change pas. sf::CircleShape recalcule la géometrie
    // a chaqur resize. Est-ce comparaison fair? Selon moi oui.
    sf::Sprite circle;
    sf::Texture circleTexture;

    sf::ConvexShape triangle;
    sf::Font font;
    sf::Text text;
};

class SystemEvent
{
public:

    void setRegistry(Registry& registry)
    {
        this->pRegistry = &registry;

        auto compPlayer = registry.getAll<PlayerComp>();
        if (compPlayer.size() > 0)
        {
            player = registry.getEntity(compPlayer[0].entityID);
        }
    }
    void update(float dt, sf::Event event) const;
private:
    Entity player;
    Registry* pRegistry;
};

static_assert(System<SystemInput>, "SystemInput must conform to the System concept.");
static_assert(System<SystemMove>, "SystemMove must conform to the System concept.");
static_assert(System<SystemCollide>, "SystemCollide must conform to the System concept.");
static_assert(System<SystemUpdate>, "SystemUpdate must conform to the System concept.");
static_assert(System<SystemRender>, "SystemUpdate must conform to the System concept.");
static_assert(System<SystemEvent>, "SystemUpdate must conform to the System concept.");



