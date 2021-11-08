#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

class Enemy 
{
private:
	float enemySpawnTimer;
	float enemySpawnTimerMax;
	int maxEnemies;

	std::vector<sf::RectangleShape> enemies;
	sf::RectangleShape enemy;

public:


};
