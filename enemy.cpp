#include <iostream>
#include <SFML/Graphics.hpp>
#include "enemy.h"

sf::RectangleShape RandomEnemySpawn(bool isSpawning)
{
	sf::RectangleShape rect((sf::Vector2f(50, 50)));
	rect.setPosition(sf::Vector2f(rand() % 400 + 300, rand() % 300 + 200));
	return rect;
}