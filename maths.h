#pragma once
#include <SFML/Graphics.hpp>
#include "struct.h"

sf::Vector2f RandomDirection();
void PlayerMovement(Player& player, float deltaTime);
void CheckAllTheCollisions(Player& player, Game& game, sf::FloatRect boundingBoxes[4]);
void MoveEnemies(Enemy& enemy, float deltaTime);
void MoveEnemyBullets(EnemyBullet& enemyBullet, float deltaTime);
void MoveBullets(Bullet& bullet, float deltaTime);
void MoveParticles(Particles& particle, float deltaTime);
void MoveBoss(Boss& boss, sf::CircleShape& player, float deltaTime);
void RotateShield(Boss& boss, float deltaTime);