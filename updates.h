#pragma once
#include <SFML/Graphics.hpp>
#include "struct.h"

void UpdatePlayerState(Player& player, float deltaTime);
void UpdateEnemyState(Enemy& enemy, float deltaTime, sf::CircleShape& player, Game& game);
void UpdateMousePos(Mouse& mouse, const sf::RenderWindow& window);
void UpdateBossState(Boss& boss, float deltaTime, Game& game, Player& player);
void UpdateBossHealth(Boss& boss);