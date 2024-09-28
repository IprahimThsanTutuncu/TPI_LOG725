#include "game_ecs.h"

GameECS::GameECS()
{
}

void GameECS::init()
{
	Entity player = registry.createEntity();
	TransformComp pt;
	pt.position = sf::Vector2f(400, 300);
	pt.radius = 30.f;
	pt.rotation = 0;
	pt.speed = 5.f;
	pt.direction = sf::Vector2f(0.f, -1.f);

	PlayerComp p;
	p.is_game_over = false;

	RenderOptionComp pro;
	pro.color = sf::Color::Red;
	pro.shape = Shape::triangle;

	registry.setComponent<TransformComp>(player, pt);
	registry.setComponent<PlayerComp>(player, p);
	registry.setComponent<RenderOptionComp>(player, pro);

	systemInput.setRegistry(registry);
	systemMove.setRegistry(registry);
	systemCollide.setRegistry(registry);
	systemUpdate.setRegistry(registry);
	systemRender.setRegistry(registry);
	systemEvent.setRegistry(registry);

	//it will spawn asteroids
	systemUpdate.update(1/60.f);

}

void GameECS::update(float dt)
{
	systemInput.update(dt);
	systemMove.update(dt);
	systemCollide.update(dt);
	systemUpdate.update(dt);
}

void GameECS::draw(sf::RenderWindow& window)
{
	systemRender.update(1 / 60.f, window);
}

void GameECS::input(sf::Event event)
{
	systemEvent.update(1/60.f, event);
}
