#pragma once
#include <SFML/Graphics.hpp>

struct Enemy {
	std::string name;
	sf::CircleShape shape;
	sf::Vector2f direction;
};

struct Item {
	std::string name;
	sf::RectangleShape shape;
	//sf::Vector2f direction;
};

struct Bullet {
	std::string name;
	sf::RectangleShape shape;
	sf::Vector2f direction;
	float rotation;
};

struct EnemyBullet {
	std::string name;
	sf::RectangleShape shape;
	sf::Vector2f direction;
	float rotation;
};