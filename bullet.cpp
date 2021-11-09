#include <iostream>
#include <SFML/Graphics.hpp>
#include "bullet.h"

void SpawnBullet(std::list<Bullet>& bullets, sf::CircleShape& player, int thickness)
{
	Bullet bullet;
	bullet.name = "bullet" + std::to_string(bullets.size());
	bullet.shape.setPosition(player.getPosition().x, player.getPosition().y);
	bullet.shape.setSize(sf::Vector2f(3, 20));
	bullet.shape.setFillColor(sf::Color::Transparent);
	bullet.shape.setOutlineThickness(2.f);
	bullet.shape.setOutlineColor(sf::Color::Green);
	bullets.push_back(bullet);
}