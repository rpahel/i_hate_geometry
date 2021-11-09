#include <iostream>
#include <SFML/Graphics.hpp>
#include "enemy.h"
#include "maths.h"

void SpawnEnemies(std::list<Enemy>& enemies, int numberOfEnemies, int thickness)
{
	enemies.resize(numberOfEnemies);
	int i = 0;

	for(auto& enemy : enemies)
	{
		enemy.name = "enemy" + std::to_string(i);
		enemy.shape.setRadius(25.f);
		enemy.shape.setOrigin(enemy.shape.getRadius(), enemy.shape.getRadius());
		enemy.direction = RandomDirection();
		enemy.shape.setPosition(rand() % (1200 - thickness * 2) + thickness, rand() % (900 - thickness * 2) + thickness);
		enemy.shape.setFillColor(sf::Color::Transparent);
		enemy.shape.setOutlineThickness(2.f);
		enemy.shape.setOutlineColor(sf::Color::Magenta);
		++i;
	}
}

void SpawnEnemiesBullet(std::list<EnemyBullet>& bullets, std::list<Enemy>& enemies, sf::CircleShape& player, int thickness)
{
	for (auto& enemy : enemies)
	{
		EnemyBullet enemyBullet;
		enemyBullet.shape.setSize(sf::Vector2f(3, 20));
		enemyBullet.shape.setOrigin(enemyBullet.shape.getSize().x / 2, enemyBullet.shape.getSize().y / 2); // change l'origine du rectangle pour être au centre
		enemyBullet.name = "enemyBullet" + std::to_string(bullets.size());
		enemyBullet.direction = player.getPosition() - enemy.position;
		float amplitude = sqrtf(enemyBullet.direction.x * enemyBullet.direction.x + enemyBullet.direction.y * enemyBullet.direction.y); // longueur du vecteur
		enemyBullet.direction = enemyBullet.direction / amplitude; // Normalisation du vecteur
		enemyBullet.rotation = std::atan2(enemyBullet.direction.x, enemyBullet.direction.y); // en radian
		enemyBullet.rotation = -enemyBullet.rotation * (180.f / 3.1415f); // Conversion en deg
		enemyBullet.shape.setRotation(enemyBullet.rotation);
		enemyBullet.shape.setPosition(enemy.position); // La balle sort du centre du cercle
		enemyBullet.shape.setFillColor(sf::Color::Transparent);
		enemyBullet.shape.setOutlineThickness(2.f);
		enemyBullet.shape.setOutlineColor(sf::Color::Yellow);
		bullets.push_back(enemyBullet);
	}
}