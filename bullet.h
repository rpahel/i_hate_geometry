#pragma once
#include <iostream>
#include <list>
#include "struct.h"

void SpawnBullet(std::list<Bullet>& bullets, sf::CircleShape& player, sf::Vector2f mousePos, int thickness);
