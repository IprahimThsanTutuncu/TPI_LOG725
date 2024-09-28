#include "system.h"
#include <SFML/Window/Keyboard.hpp>
#include <cmath>
#include <random>

void SystemInput::update(float dt)
{
    static TransformComp& t = pRegistry->getComponent<TransformComp>(player);

    auto updateRotationAndDirection = [&](float angleChange) {
        t.rotation += angleChange;

        float radians = t.rotation * (3.14159f / 180.f);
        float cosAngle = std::cos(radians);
        float sinAngle = std::sin(radians);

        t.direction = sf::Vector2f(
            sinAngle,
            -cosAngle
        );
        };

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        updateRotationAndDirection(-10.f * dt);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        updateRotationAndDirection(10.f * dt);
    }
}


void SystemMove::update(float dt)
{
    auto& transforms = pRegistry->getAll<TransformComp>();
    for (TransformComp& c : transforms)
    {
        if (pRegistry->componentIsValid(c)) {

            c.position += c.direction * c.speed * dt;
        }
    }
}

void SystemCollide::update(float dt)
{
    auto& bullets = pRegistry->getAll<BulletComp>();
    PlayerComp& player = pRegistry->getAll<PlayerComp>()[0];
    auto& asteroids = pRegistry->getAll<AsteroidComp>();

    auto& pc = pRegistry->getComponent<TransformComp>(player.entityID);

    for (AsteroidComp& a : asteroids)
    {
        if (pRegistry->componentIsValid(a)) {
            auto& ac = pRegistry->getComponent<TransformComp>(a.entityID);

            for (BulletComp& b : bullets)
            {
                if (b.entityID != -1) 
                {
                    auto& bc = pRegistry->getComponent<TransformComp>(b.entityID);
                    float dx = ac.position.x - bc.position.x;
                    float dy = ac.position.y - bc.position.y;
                    float distance = std::sqrt(dx * dx + dy * dy);

                    if (distance < (ac.radius + bc.radius))
                    {
                        a.is_destroyed = true;
                        pRegistry->remove(a.entityID);
                    }
                }
            }

            float playerDx = ac.position.x - pc.position.x;
            float playerDy = ac.position.y - pc.position.y;
            float playerDistance = std::sqrt(playerDx * playerDx + playerDy * playerDy);

            if (playerDistance < (ac.radius + pc.radius))
            {
                player.is_game_over = true;
            }
        }
    }
}

void SystemUpdate::update(float dt)
{
    auto& bullets = pRegistry->getAll<BulletComp>();
    for (BulletComp& b : bullets)
    {
        if (pRegistry->componentIsValid(b)) {
            if (b.entityID != -1)
            {
                b.life_time -= dt;
                if (b.life_time < 0.)
                    b.is_destroyed = true;

                if (b.is_destroyed)
                    pRegistry->remove(b.entityID);
            }
        }
    }

    auto& asteroids = pRegistry->getAll<AsteroidComp>();
    for (AsteroidComp& a : asteroids)
    {
        if (!pRegistry->componentIsValid(a))
            continue;

        auto& t = pRegistry->getComponent<TransformComp>(a.entityID);

        t.direction += sf::Vector2f((std::rand() % 100 - 50) / 1000.f, (std::rand() % 100 - 50) / 1000.f);

        float length = std::sqrt(t.direction.x * t.direction.x + t.direction.y * t.direction.y);
        if (length > 0) 
        {
            t.direction /= length;
        }

        t.rotation = 0; // I mean c'est un cercle lol
    }

    PlayerComp& player = pRegistry->getAll<PlayerComp>()[0];
    auto& player_transform = pRegistry->getComponent<TransformComp>(player.entityID);

    int total = pRegistry->countEntitiesWithComponent<AsteroidComp>();
    int spawn_count = 50 - total;
    for (int i = 0; i < spawn_count; i++)
    {
        float distance = 0.f;
        float radius = 0.f;
        sf::Vector2f position;

        do
        {
           radius = rand() % 30 + 20;
           position= sf::Vector2f(
                static_cast<float>(rand() % (800 - static_cast<int>(radius * 2)) + radius),
                static_cast<float>(rand() % (600 - static_cast<int>(radius * 2)) + radius)
            );

            sf::Vector2f playerPos = player_transform.position;

            distance = std::hypot(position.x - playerPos.x, position.y - playerPos.y);

        } while (distance < 2*(radius + player_transform.radius));

        AsteroidComp a;
        a.is_destroyed = false;

        TransformComp t;
        t.radius = radius;
        t.position = position;
        t.speed = std::max(1.f, 10.f / t.radius);
        t.rotation = 0;

        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> colorDist(100, 200);
        sf::Color color(colorDist(rng), colorDist(rng), colorDist(rng));

        RenderOptionComp ro;
        ro.color = color;
        ro.shape = Shape::circle;

        Entity e = pRegistry->createEntity();
        pRegistry->setComponent<AsteroidComp>(e, a);
        pRegistry->setComponent<TransformComp>(e, t);
        pRegistry->setComponent<RenderOptionComp>(e, ro);
    }

}

void SystemRender::update(float dt, sf::RenderWindow& window)
{
    const auto& rendereables = pRegistry->getAll<RenderOptionComp>();
    window.clear();

    for (const auto& r : rendereables)
    {
        if (pRegistry->componentIsValid(r))
        {
            Entity e = pRegistry->getEntity(r.entityID);
            auto& t  = pRegistry->getComponent<TransformComp>(e);
            
            if (r.shape == Shape::circle)
            {
                circle.setPosition(t.position);
                circle.setScale(t.radius / (512.f / 2), t.radius / (512.f / 2));
                circle.setColor(r.color);
                circle.setRotation(t.rotation);
                circle.setOrigin(256, 256);

                window.draw(circle);
            }
            else if(r.shape == Shape::triangle)
            {
                triangle.setPosition(t.position);
                triangle.setFillColor(r.color);
                triangle.setRotation(t.rotation);

                window.draw(triangle);

                //si triangle, forcément un player
                //pas mon meilleur code
                auto& p = pRegistry->getComponent<PlayerComp>(e);
                if (p.is_game_over)
                {
                    window.draw(text);
                }
            }
        }
    }

    window.display();

}

void SystemEvent::update(float dt, sf::Event event) const
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Space)
        {
            auto& pt = pRegistry->getComponent<TransformComp>(player);
            
            Entity e = pRegistry->createEntity();
            BulletComp bc;
            bc.is_destroyed = false;
            bc.life_time = 20.f;
            
            TransformComp tc;
            tc.position = pt.position;
            tc.radius = 5;
            tc.direction = pt.direction;
            tc.rotation = pt.rotation;
            tc.scale = pt.scale;
            tc.speed = 20.f;
            RenderOptionComp rc;
            rc.color = sf::Color::Yellow;
            rc.shape = Shape::circle;

            pRegistry->setComponent<BulletComp>(e, bc);
            pRegistry->setComponent<TransformComp>(e, tc);
            pRegistry->setComponent<RenderOptionComp>(e, rc);
        }
    }
}
