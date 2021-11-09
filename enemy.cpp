#include <iostream>
#include <SFML/Graphics.hpp>
#include "enemy.h"
#include "maths.h"

void SpawnEnemies(std::list<Enemy>& enemies, int numberOfEnemies, int thickness)
{
	enemies.resize(numberOfEnemies);
	int i = 0;

	for(auto& enemy : enemies)
	{
		enemy.name = "enemy" + std::to_string(i);
		enemy.shape.setRadius(25.f);
		enemy.shape.setOrigin(enemy.shape.getRadius(), enemy.shape.getRadius());
		enemy.direction = RandomDirection();
		enemy.shape.setPosition(rand() % (1200 - thickness * 2) + thickness, rand() % (900 - thickness * 2) + thickness);
		enemy.shape.setFillColor(sf::Color::Transparent);
		enemy.shape.setOutlineThickness(2.f);
		enemy.shape.setOutlineColor(sf::Color::Magenta);
		++i;
	}
}
