#include <SFML/Graphics.hpp>
#include "struct.h"
#include <iostream>

void UpdatePlayerState(Player& player, float deltaTime)
{
	player.fireCD -= deltaTime;
}

void UpdateEnemyState(Enemy& enemy, float deltaTime)
{
	enemy.fireCD -= deltaTime;
	enemy.moveCD -= deltaTime;
}

void UpdateMousePos(Mouse& mouse, const sf::RenderWindow& window)
{
	mouse.x = sf::Mouse::getPosition(window).x;
	mouse.y = sf::Mouse::getPosition(window).y;
}

void UpdateBossState(Boss& boss, float deltaTime)
{
	boss.timeBeforeUpdate -= deltaTime;
	boss.fireCD -= deltaTime;
	int state = rand()% 3;

	if (boss.timeBeforeUpdate <= 0)
	{
		if (state == 0)
		{
			std::cout << "0" << std::endl;
			boss.myState = boss.isMoving;
		}

		if (state == 1)
		{
			std::cout << "1" << std::endl;
			boss.myState = boss.isShooting;
		}

		if (state == 2)
		{
			std::cout << "2" << std::endl;
			boss.myState = boss.isBlocking;
		}

		boss.timeBeforeUpdate = 2.0f;
	}
}