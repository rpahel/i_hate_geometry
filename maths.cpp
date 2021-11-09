#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>
#include "struct.h"

// Gestion des d�placement d'un player
void PlayerMovement(sf::CircleShape& circle, float deltaTime)
{
	float speed = 300.f; // 600 pixels par seconde

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		circle.move(sf::Vector2f(0.f, -speed * deltaTime));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		circle.move(sf::Vector2f(0.f, speed * deltaTime));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		circle.move(sf::Vector2f(-speed * deltaTime, 0.f));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		circle.move(sf::Vector2f(speed * deltaTime, 0.f));
}

void CheckWallCollision(sf::CircleShape& object, sf::FloatRect boundingBoxes[4], float deltaTime)
{
	float force = 5.0f;
	sf::FloatRect playerBox = object.getGlobalBounds();

	if (playerBox.intersects(boundingBoxes[0])) //Sud
	{
		object.move(sf::Vector2f(0, -1) * force);
	}

	if (playerBox.intersects(boundingBoxes[1])) //Nord
	{
		object.move(sf::Vector2f(0, 1) * force);
	}

	if (playerBox.intersects(boundingBoxes[2])) //Est
	{
		object.move(sf::Vector2f(-1, 0) * force);
	}

	if (playerBox.intersects(boundingBoxes[3])) //West
	{
		object.move(sf::Vector2f(1, 0) * force);
	}
}

// Gestion des collisions
void CheckCollision(sf::CircleShape& objectA, sf::CircleShape& objectB, float deltaTime)
{
	float speed = 12.f;
	// Check si les deux cercles se superposent
	if((objectA.getPosition().x - objectB.getPosition().x) * (objectA.getPosition().x - objectB.getPosition().x) + 
		(objectA.getPosition().y - objectB.getPosition().y) * (objectA.getPosition().y - objectB.getPosition().y) <= 
		(objectA.getRadius() + objectB.getRadius()) * (objectA.getRadius() + objectB.getRadius())) // dX^2 + dY^2 <= (R1 - R2)^2
	{
		// S'ils se superposent, les d�placer pour qu'ils ne se superposent plus
		sf::Vector2f playerToObject = objectB.getPosition() - objectA.getPosition();
		objectA.move(playerToObject * -0.5f * deltaTime * speed);
		objectB.move(playerToObject * 0.5f * deltaTime * speed);
	}
}

void CheckAllTheCollisions(sf::CircleShape& player, std::list<Enemy>& enemies, sf::FloatRect boundingBoxes[4], float deltaTime)
{
	// On check les collisions entre le joueur et les murs
	CheckWallCollision(player, boundingBoxes, deltaTime);

	// Pour chaque enemy, on check la collision avec le joueur + on check la collision avec un mur
	for(auto it = enemies.begin(); it != enemies.end(); ++it)
	{
		CheckCollision(player, it->shape, deltaTime);
		CheckWallCollision(it->shape, boundingBoxes, deltaTime);
	}
}

sf::Vector2f RandomDirection()
{
	int tab[2]{ -1, 1 };
	sf::Vector2f direction((tab[rand() % 2]), (tab[rand() % 2])); // Vecteur de direction qui peut valoir (0,0) (-1,0) (1,0) (1,1) etc..
	return direction;
}

void MoveEnemies(sf::CircleShape& enemy, const sf::Vector2f& direction, float deltaTime)
{
	float speed = 120.f;
	enemy.move(direction * speed * deltaTime);
}

void ChangeEnemyDirection(sf::Vector2f& direction)
{
	direction = RandomDirection();
}

void MoveBullets(sf::RectangleShape& bullet, const sf::Vector2f& direction, float rotation, float deltaTime)
{
	float speed = 2.0f;
	bullet.move(direction * speed * deltaTime);
}