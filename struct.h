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

struct Boss {
	std::string name;
	sf::CircleShape shape;
	sf::Vector2f direction;
	int type;
	int health;
	int state;
	float bossSpeed;
	float fireRate;
	float fireCD;
	float fireSpeed;
	float changeStateTime;
	bool shieldsUp;
	bool isFiring;
	bool isMoving;
	bool CacUp;
};

struct BossCAC
{
	std::string name;
	sf::RectangleShape shape;
	sf::Vector2f direction;
	float rotation;
};

struct BossShield {
	std::string name;
	sf::CircleShape shape;
	float speed;
	sf::Vector2f direction;
};

struct Game {
	std::list<Enemy> enemies;	
	std::list<Bullet> bullets;
	std::list<EnemyBullet> enemyBullet;
	std::list<Item> items;
	std::list<Particles> particles;
	std::list<Boss> bosses;
	std::list<BossShield> bossShields;
	std::list<BossCAC> bossCacs;
	int currentLevel;
	bool isNewRoom;
	sf::Font font;
	sf::Text levelText;
	sf::Clock clock;
	int numberOfEnemies;
	int numberOfItems;
	sf::Time deltaTime;
	float timeSinceStartLevel;
	sf::Text pauseText;
	sf::RectangleShape button1;
	sf::Text restartText;
	sf::RectangleShape button2;
	sf::Text quitText;
	bool isPaused;
};

struct Mouse
{
	int x;
	int y;
};