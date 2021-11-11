#pragma once
#include <SFML/Graphics.hpp>
#include <list>

struct Player {
	sf::CircleShape shape;
	float fireRate;
	float playerSpeed;
	float radius;
	int numberOfBullets;
};

struct Enemy {
	std::string name;
	sf::CircleShape shape;
	sf::Vector2f direction;
};

struct Item {
	std::string name;
	std::string effect;
	sf::RectangleShape shape;
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

struct Particles {
	std::string name;
	sf::RectangleShape shape;
	sf::Vector2f direction;
	float rotation;
	float lifeTime;
};

struct Game {
	std::list<Enemy> enemies;	
	std::list<Bullet> bullets;
	std::list<EnemyBullet> enemyBullet;
	std::list<Item> items;
	std::list<Particles> particles;
};