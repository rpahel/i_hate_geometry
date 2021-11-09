#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "struct.h"

void PlayerMovement(sf::CircleShape& player, float deltaTime);
void CheckAllTheCollisions(sf::CircleShape& player, std::list<Enemy>& enemies, sf::FloatRect boundingBoxes[4], float deltaTime);
sf::Vector2f RandomDirection();
void MoveEnemies(sf::CircleShape& enemy, const sf::Vector2f& direction, float deltaTime);