#include <SFML/Graphics.hpp>
#include "struct.h"

void UpdatePlayerState(Player& player, float deltaTime)
{
	player.fireCD -= deltaTime;
}

void UpdateEnemyState(Enemy& enemy, float deltaTime)
{
	enemy.fireCD -= deltaTime;
}

void UpdateMousePos(Mouse& mouse, const sf::RenderWindow& window)
{
	mouse.x = sf::Mouse::getPosition(window).x;
	mouse.y = sf::Mouse::getPosition(window).y;
}