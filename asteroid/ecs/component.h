#pragma once
#include <string>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

template<typename T>
concept Component = requires(T t) {
    { t.entityID };
};

enum class ComponentType {
    Player = 0,
    Asteroid = 1,
    Bullet = 2,
    Transform = 3,
    RenderOption = 4,
    LAST = 5
};

struct TransformComp {
    sf::Vector2f position;
    sf::Vector2f scale;
    sf::Vector2f direction;
    
    float rotation;
    float radius;
    float speed;
    int entityID{-1};
};

struct PlayerComp {  
    int entityID{-1};
    bool is_game_over{ false };
};

struct AsteroidComp {
    bool is_destroyed{ false };
    int entityID{-1};
};

struct BulletComp {
    int bullet_id;
    bool is_destroyed{ false };
    float life_time;
    int entityID{-1};
};

enum class Shape
{
    triangle,
    circle
};

struct RenderOptionComp
{
    int entityID{ -1 };
    sf::Color color;
    Shape shape;
};
