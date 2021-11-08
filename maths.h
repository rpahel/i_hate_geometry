#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

void PlayerMovement(sf::CircleShape& player, float deltaTime);
void CheckCollision(sf::CircleShape& player, sf::CircleShape& object, float deltaTime);