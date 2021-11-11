#pragma once
#include <SFML/Graphics.hpp>
#include <list>

struct Player {
	float radius;
	sf::CircleShape shape;
	float fireRate;
	float fireCD;
	float playerSpeed;
	int numberOfBullets;
	bool isDead;
};

struct Enemy {
	std::string name;
	int type;
	sf::CircleShape shape;
	sf::Vector2f direction;
	float enemySpeed;
	float fireRate;
	float fireCD;
	float moveDuration;
	float moveCD;
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
	float bulletSpeed;
};

struct EnemyBullet {
	std::string name;
	sf::RectangleShape shape;
	sf::Vector2f direction;
	float rotation;
	float bulletSpeed;
};

struct Particles {
	std::string name;
	sf::RectangleShape shape;
	sf::Vector2f direction;
	float rotation;
	float lifeTime;
	float particleSpeed;
};

struct Game {
	std::list<Enemy> enemies;	
	std::list<Bullet> bullets;
	std::list<EnemyBullet> enemyBullet;
	std::list<Item> items;
	std::list<Particles> particles;
	int currentLevel;
	bool isNewRoom;
	sf::Font font;
	sf::Text levelText;
	sf::Clock clock;
	int numberOfEnemies;
	int numberOfItems;
	sf::Time deltaTime;
	float timeSinceStartLevel;
};

struct Mouse
{
	int x;
	int y;
};