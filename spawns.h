#pragma once
#include "struct.h"

Player SpawnPlayer();
void SpawnEnemies(Game& game, int wallThickness);
void SpawnEnemiesBullet(Game& game, Enemy& enemy, sf::CircleShape& player, int bulletNumber);
void SpawnBullet(Game& game, const Player& player, const Mouse& mouse);
void SpawnItems(Game& game, int wallThickness);
void SpawnParticles(const Enemy& enemy, Game& game);
void SpawnPlayerParticles(const Player& player, Game& game);
void SpawnBoss(Game& game, int wallThickness);
void SpawnBossBullet(Game& game, Boss& boss, sf::CircleShape& player, int bulletNumber);
void RestartGame(Game& game, Player& player);
void SpawnBossShield(Game& game, Boss& boss);