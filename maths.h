#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "struct.h"

sf::Vector2f RandomDirection();
void PlayerMovement(sf::CircleShape& player, float& speed, float deltaTime);
void CheckAllTheCollisions(sf::CircleShape& player, std::list<Enemy>& enemies, sf::FloatRect boundingBoxes[4], float deltaTime);
void MoveEnemies(sf::CircleShape& enemy, const sf::Vector2f& direction, float deltaTime);
void ChangeEnemyDirection(sf::Vector2f& direction);
void MoveEnemyBullets(sf::RectangleShape& bullet, const sf::Vector2f& direction, float rotation, float deltaTime);
void MoveBullets(sf::RectangleShape& bullet, const sf::Vector2f& direction, float rotation, float deltaTime);