#include <iostream>
#include <SFML/Graphics.hpp>
#include "bullet.h"

void SpawnBullet(std::list<sf::RectangleShape>& bullets, sf::CircleShape& player)
{
	sf::RectangleShape bullet;
	bullet.setSize(sf::Vector2f(50, 50));
	bullet.setFillColor(sf::Color::Green);
	bullet.setPosition(player.getPosition().x, player.getPosition().y - 50);
	bullets.push_back(bullet);
}