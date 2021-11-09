#pragma once
#include <SFML/Graphics.hpp>

struct Enemy {
	std::string name;
	sf::CircleShape shape;
	sf::Vector2f direction;
};