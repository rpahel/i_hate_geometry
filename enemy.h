#pragma once
#include <iostream>
#include <list>
#include "struct.h"

void SpawnEnemies(Game &game, int numberOfEnemies, int thickness);

void SpawnEnemiesBullet(Game &game, Enemy &enemy, sf::CircleShape& player, int thickness);