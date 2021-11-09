#pragma once
#include <iostream>
#include <list>
#include "struct.h"

void SpawnEnemies(std::list<Enemy>& enemies, int numberOfEnemies, int thickness);

void SpawnEnemiesBullet(std::list<EnemyBullet>& bullets, std::list<Enemy>& enemies, sf::CircleShape& player, int thickness);