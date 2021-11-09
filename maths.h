#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

void PlayerMovement(sf::CircleShape& player, float deltaTime);
void CheckAllTheCollisions(sf::CircleShape& player, std::list<sf::CircleShape>& enemies, sf::FloatRect boundingBoxes[4], float deltaTime);