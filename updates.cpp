#include <SFML/Graphics.hpp>
#include "struct.h"
#include "spawns.h"
#include "maths.h"
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

void UpdateBossState(Boss& boss, float deltaTime, Game& game)
{
	boss.changeStateTime -= deltaTime;
	boss.fireCD -= deltaTime;

	if (boss.changeStateTime <= 0)
	{
		int state = 2;

		if (state == 0)
		{
			boss.shieldsUp = false;
		}

		if (state == 1)
		{
			boss.shieldsUp = false;
		}

		if (state == 2) // Shields
		{
			if(!boss.shieldsUp)
			{
				SpawnBossShield(boss, game);
			}

			boss.shieldsUp = true;
		}

		if (!boss.shieldsUp)
		{
			game.bossShields.clear();
		}

		boss.changeStateTime = 5.f;
	}

	for(auto& shield : game.bossShields)
	{
		RotateShield(boss, shield, deltaTime);
	}
}