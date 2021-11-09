#include <iostream>
#include <SFML/Graphics.hpp>
#include "enemy.h"

sf::RectangleShape RandomEnemySpawn()
{
	sf::RectangleShape rect((sf::Vector2f(50, 50)));
	rect.setPosition(sf::Vector2f(rand() % 400 + 300, rand() % 300 + 200));
	rect.setFillColor(sf::Color::Red);
	return rect;
}

void SpawnEnemies(std::list<sf::CircleShape>& enemies, int numberOfEnemies, int thickness)
{
	for(int i = 0; i < numberOfEnemies; i++)
	{
		sf::CircleShape enemy;
		enemy.setRadius(25);
		enemy.setFillColor(sf::Color::Transparent);
		enemy.setOutlineThickness(2);
		enemy.setOutlineColor(sf::Color::Magenta);
		enemy.setPosition(rand() % (1200 - thickness * 2) + thickness, rand() % (900 - thickness * 2) + thickness);
		enemies.push_back(enemy);
	}
}