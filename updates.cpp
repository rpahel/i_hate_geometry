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

void UpdateBossState(Boss& boss, float deltaTime, Game& game, Player& player)
{
	boss.changeStateTime -= deltaTime;
	boss.fireCD -= deltaTime;
	boss.fireSpeed -= deltaTime;
	boss.previousState = boss.state;

	if (boss.changeStateTime <= 0)
	{

		if (boss.state == 0)
		{
			boss.isMoving = true;
			boss.shieldsUp = false;
			boss.isFiring = false;
		}

		if (boss.state == 1)
		{
			boss.isMoving = false;
			boss.shieldsUp = false;
			boss.isFiring = true;
			boss.CacUp = false;
		}

		if (boss.state == 2) // Shields
		{
			if(!boss.shieldsUp)
			{
				SpawnBossShield(boss, game);
			}

			boss.isMoving = false;
			boss.shieldsUp = true;
			boss.isFiring = false;
			boss.CacUp = false;
		}

		if (!boss.shieldsUp)
		{
			game.bossShields.clear();
		}

		if(!boss.CacUp)
		{
			game.bossCacs.clear();
		}

		boss.changeStateTime = (rand() % 300 + 300) / 100;

		while (boss.state == boss.previousState)
		{
			boss.state = rand() % 3;
		}
	}

	if(boss.isMoving)
	{
		MoveBoss(boss, player.shape, deltaTime);

		if (!boss.CacUp)
		{
			SpawnBossCAC(game, boss, 12);
			boss.CacUp = true;
		}
	}

	if(boss.isFiring && boss.fireSpeed <= 0.f)
	{
		int numberOfBullets = rand() % 17 + 3;
		SpawnBossBullet(game, boss, numberOfBullets);
		boss.fireSpeed = .75f;
	}

	for(auto& shield : game.bossShields)
	{
		RotateShield(boss, shield, deltaTime);
	}

	for (auto& cac : game.bossCacs)
	{
		MoveCACs(cac, boss);
	}
}

void UpdateBossHealth(Boss& boss)
{
	boss.shapeHealthBar.setSize(sf::Vector2f((float(boss.health) / float(boss.maxHealth)) * 100.f, 10.f));
}