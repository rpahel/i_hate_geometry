#include <iostream>
#include <SFML/Graphics.hpp>

sf::CircleShape SpawnPlayer()
{
	sf::CircleShape player(25);
	player.setOrigin(player.getRadius(), player.getRadius());
	player.setFillColor(sf::Color::Transparent);
	player.setOutlineThickness(2);
	player.setOutlineColor(sf::Color::Cyan);
	player.setPosition(600, 450);
	return player;
}