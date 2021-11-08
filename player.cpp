#include <iostream>
#include <SFML/Graphics.hpp>

sf::CircleShape SpawnPlayer()
{
	sf::CircleShape player(25);
	player.setFillColor(sf::Color::Transparent);
	player.setOutlineThickness(2);
	player.setOutlineColor(sf::Color::Cyan);
	player.setPosition(575, 425);
	return player;
}