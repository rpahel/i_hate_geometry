#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>
#include "struct.h"
#include "particle.h"
#include <math.h>

float car(float a)
{
	float b = a * a;
	return b;
}

float distance(float A, float B)
{
	float D = sqrt(car(A) + car(B));
	return D;
}

// Gestion des d�placement d'un player
void PlayerMovement(sf::CircleShape& circle, float& speed, float deltaTime) //300 speed = 600pixels par seconde
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		circle.move(sf::Vector2f(0.f, -speed * deltaTime));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		circle.move(sf::Vector2f(0.f, speed * deltaTime));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		circle.move(sf::Vector2f(-speed * deltaTime, 0.f));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		circle.move(sf::Vector2f(speed * deltaTime, 0.f));
}

void CheckPlayerWallCollision(sf::CircleShape& object, sf::FloatRect boundingBoxes[4], float& speed)
{
	float force = 0.009f * speed;
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

void CheckEnemyWallCollision(Enemy& enemy, sf::FloatRect boundingBoxes[4])
{
	sf::FloatRect enemyBox = enemy.shape.getGlobalBounds();

	if (enemyBox.intersects(boundingBoxes[0]) || enemyBox.intersects(boundingBoxes[1])) // Sud || Nord
	{
		enemy.direction = sf::Vector2f(enemy.direction.x, -enemy.direction.y);
	}

	if (enemyBox.intersects(boundingBoxes[2]) || enemyBox.intersects(boundingBoxes[3])) // Est || West
	{
		enemy.direction = sf::Vector2f(-enemy.direction.x, enemy.direction.y);
	}
}

void CheckItemWallCollision(sf::RectangleShape& item, sf::FloatRect boundingBoxes[4])
{
	sf::FloatRect itemBox = item.getGlobalBounds();

	if (itemBox.intersects(boundingBoxes[0])) //Sud
	{
		item.move(sf::Vector2f(0, -1));
	}

	if (itemBox.intersects(boundingBoxes[1])) //Nord
	{
		item.move(sf::Vector2f(0, 1));
	}

	if (itemBox.intersects(boundingBoxes[2])) //Est
	{
		item.move(sf::Vector2f(-1, 0));
	}

	if (itemBox.intersects(boundingBoxes[3])) //West
	{
		item.move(sf::Vector2f(1, 0));
	}
}

bool CheckBulletWallCollision(sf::RectangleShape bullet, sf::FloatRect boundingBoxes[4])
{
	sf::FloatRect bulletBox = bullet.getGlobalBounds();

	if (bulletBox.intersects(boundingBoxes[0]) || bulletBox.intersects(boundingBoxes[1]) || bulletBox.intersects(boundingBoxes[2]) || bulletBox.intersects(boundingBoxes[3]))
	{
		return true;
	}

	return false;
}

// Gestion des collisions
void CheckCollision(sf::CircleShape& objectA, sf::CircleShape& objectB, float deltaTime)
{
	float speed = 12.f;
	// Check si les deux cercles se superposent
	if(distance(objectA.getPosition().x - objectB.getPosition().x, objectA.getPosition().y - objectB.getPosition().y) <= objectA.getRadius() + objectB.getRadius()) // dX^2 + dY^2 <= (R1 - R2)^2
	{
		// S'ils se superposent, les d�placer pour qu'ils ne se superposent plus
		sf::Vector2f playerToObject = objectB.getPosition() - objectA.getPosition();
		objectA.move(playerToObject * -0.5f * deltaTime * speed);
		objectB.move(playerToObject * 0.5f * deltaTime * speed);
	}
}

void CheckPlayerBulletCollision(sf::CircleShape& player, std::list<EnemyBullet>& enemyBullets, bool& isDead)
{
	for(auto pew = enemyBullets.begin(); pew != enemyBullets.end();)
	{
		float D = distance(player.getPosition().x - pew->shape.getPosition().x, player.getPosition().y - pew->shape.getPosition().y);
		if(D <= player.getRadius())
		{
			isDead = true;
			pew = enemyBullets.erase(pew);
		}
		else
		{
			++pew;
		}
	}
}

bool CheckPlayerItemCollision(sf::CircleShape& player, Item& item, float& speed)
{
	if (distance(player.getPosition().x - item.shape.getPosition().x, player.getPosition().y - item.shape.getPosition().y) <= player.getRadius())
	{
		if(item.effect == "speedUp")
		{
			speed *= 1.5f;
		}
		if(item.effect == "speedDown")
		{
			speed *= .75f;
		}
		return true;
	}

	return false;
}

bool CheckEnemyBulletCollision(sf::CircleShape& enemy, std::list<Bullet>& bullets)
{
	for(auto pew = bullets.begin(); pew != bullets.end();)
	{
		float D = distance(enemy.getPosition().x - pew->shape.getPosition().x, enemy.getPosition().y - pew->shape.getPosition().y);
		if(D <= enemy.getRadius())
		{
			pew = bullets.erase(pew);
			return true;
		}

		++pew;
	}

	return false;
}

void CheckAllTheCollisions(sf::CircleShape& player, std::list<Enemy>& enemies, sf::FloatRect boundingBoxes[4], std::list<Bullet>& bullets, std::list<EnemyBullet>& enemyBullets, std::list<Item>& items, float& playerSpeed, bool& isDead, float deltaTime)
{
	// On check les collisions entre le joueur et les murs
	CheckPlayerWallCollision(player, boundingBoxes, playerSpeed);
	CheckPlayerBulletCollision(player, enemyBullets, isDead);

	// Pour chaque enemy, on check la collision avec le joueur + on check la collision avec un mur
	for(auto it = enemies.begin(); it != enemies.end();)
	{
		CheckCollision(player, it->shape, deltaTime);
		CheckEnemyWallCollision(*it, boundingBoxes);
		CheckPlayerWallCollision(it->shape, boundingBoxes, playerSpeed); //Cette fonction evite que les ennemis qui spawnent dans le mur restent coincés dedans
		if(CheckEnemyBulletCollision(it->shape, bullets))
		{
			SpawnParticles(it->shape.getPosition());
			it = enemies.erase(it);
		}
		else
		{
			++it;
		}
	}

	for(auto it = bullets.begin(); it != bullets.end();)
	{
		if (CheckBulletWallCollision(it->shape, boundingBoxes))
		{
			it = bullets.erase(it);
		}
		else
		{
			++it;
		}
	}

	for (auto it = enemyBullets.begin(); it != enemyBullets.end();)
	{
		if (CheckBulletWallCollision(it->shape, boundingBoxes))
		{
			it = enemyBullets.erase(it);
		}
		else
		{
			++it;
		}
	}

	for (auto it = items.begin(); it != items.end();)
	{
		CheckItemWallCollision(it->shape, boundingBoxes);
		if(CheckPlayerItemCollision(player, *it, playerSpeed))
		{
			it = items.erase(it);
		}
		else
		{
			++it;
		}
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
	if((rand() % 3) + 1 == 1) // Chaque ennemi a une chance sur 3 de changer de direction quand la fonction est appelée
	{
		direction = RandomDirection();
	}
}

void MoveEnemyBullets(sf::RectangleShape& bullet, const sf::Vector2f& direction, float rotation, float deltaTime)
{
	float speed = 400.0f;
	bullet.move(direction * speed * deltaTime);
}

void MoveBullets(sf::RectangleShape& bullet, const sf::Vector2f& direction, float rotation, float deltaTime)
{
	float speed = 800.0f;
	bullet.move(direction * speed * deltaTime);
}