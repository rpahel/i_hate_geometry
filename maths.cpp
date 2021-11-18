#include <SFML/Graphics.hpp>
#include <list>
#include <iostream>
#include "spawns.h"
#include "struct.h"

float car(float a) // fonction carré
{
	float b = a * a;
	return b;
}

float pyth(float A, float B) // fonction theoreme de Pythagore
{
	float D = sqrt(car(A) + car(B));
	return D;
}

// Gestion des deplacement du Player
void PlayerMovement(Player& player, float deltaTime)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) // Si le joueur appuie sur Up ou Z...
		player.shape.move(sf::Vector2f(0.f, -player.playerSpeed * deltaTime)); // Le joueur va vers le haut

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) // Si le joueur appuie sur Down ou S...
		player.shape.move(sf::Vector2f(0.f, player.playerSpeed * deltaTime)); // Le joueur va vers le bas

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) // Si le joueur appuie sur Gauche ou Q...
		player.shape.move(sf::Vector2f(-player.playerSpeed * deltaTime, 0.f)); // Le joueur va vers la gauche

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) // Si le joueur appuie sur Droite ou D...
		player.shape.move(sf::Vector2f(player.playerSpeed * deltaTime, 0.f)); // Le joueur va vers la droite
}

// Gestion des collisions Joueur - Murs
void CheckPlayerWallCollision(sf::CircleShape& object, sf::FloatRect boundingBoxes[4], float& speed)
{
	float force = 0.015f * speed; // Force de répulsion du mur (je sais pas pourquoi mais cette valeur est celle qui faut)
	sf::FloatRect playerBox = object.getGlobalBounds(); // On prend la hitBox du joueur

	if (playerBox.intersects(boundingBoxes[0])) //Si le joueur percute le mur Sud...
	{
		object.setPosition(object.getPosition().x, 900 - boundingBoxes[0].height - object.getRadius() - 1); // On le place juste avant le mur
	}

	if (playerBox.intersects(boundingBoxes[1])) //Si le joueur percute le mur Nord...
	{
		object.setPosition(object.getPosition().x, boundingBoxes[1].height + object.getRadius() + 1); // On le place juste avant le mur
	}

	if (playerBox.intersects(boundingBoxes[2])) //Si le joueur percute le mur Est...
	{
		object.setPosition(1200 - boundingBoxes[2].width - object.getRadius() - 1, object.getPosition().y); // On le place juste avant le mur
	}

	if (playerBox.intersects(boundingBoxes[3])) //Si le joueur percute le mur West...
	{
		object.setPosition(boundingBoxes[3].width + object.getRadius() + 1, object.getPosition().y); // On le place juste avant le mur
	}
}

// Gestion des collisions Ennemi - Murs
void CheckEnemyWallCollision(Enemy& enemy, sf::FloatRect boundingBoxes[4])
{
	sf::FloatRect enemyBox = enemy.shape.getGlobalBounds(); // On prend la hitbox de l'ennemi

	if (enemyBox.intersects(boundingBoxes[0]) || enemyBox.intersects(boundingBoxes[1])) // Sud || Nord
	{
		enemy.direction = sf::Vector2f(enemy.direction.x, -enemy.direction.y); // On inverse la composante verticale de la direction
	}

	if (enemyBox.intersects(boundingBoxes[2]) || enemyBox.intersects(boundingBoxes[3])) // Est || West
	{
		enemy.direction = sf::Vector2f(-enemy.direction.x, enemy.direction.y); // On inverse la composante horizontale de la direction
	}
}

// Gestion des collisions Item - Murs
void CheckItemWallCollision(sf::RectangleShape& item, sf::FloatRect boundingBoxes[4])
{
	sf::FloatRect itemBox = item.getGlobalBounds(); // On prend la hitbox de l'item

	if (itemBox.intersects(boundingBoxes[0])) //Pareil que pour le joueur
	{
		item.move(sf::Vector2f(0, -1));
	}

	if (itemBox.intersects(boundingBoxes[1]))
	{
		item.move(sf::Vector2f(0, 1));
	}

	if (itemBox.intersects(boundingBoxes[2]))
	{
		item.move(sf::Vector2f(-1, 0));
	}

	if (itemBox.intersects(boundingBoxes[3]))
	{
		item.move(sf::Vector2f(1, 0));
	}
}

// Check des collisions Balle - Murs
bool CheckBulletWallCollision(sf::RectangleShape bullet, sf::FloatRect boundingBoxes[4])
{
	sf::FloatRect bulletBox = bullet.getGlobalBounds(); // On prend la hitbox de la balle

	if (bulletBox.intersects(boundingBoxes[0]) || bulletBox.intersects(boundingBoxes[1]) || bulletBox.intersects(boundingBoxes[2]) || bulletBox.intersects(boundingBoxes[3])) // Si la balle touche n'importe quel mur...
	{
		return true; // On renvoit true
	}

	return false; // On renvoit false
}

// Gestion des collisions Joueur - Ennemis
void CheckCollision(sf::CircleShape& objectA, sf::CircleShape& objectB, float deltaTime)
{
	float speed = 12.f;
	// Check si les deux cercles se superposent
	if(pyth(objectA.getPosition().x - objectB.getPosition().x, objectA.getPosition().y - objectB.getPosition().y) <= objectA.getRadius() + objectB.getRadius()) // Si les deux objets se superposent...
	{
		sf::Vector2f playerToObject = objectB.getPosition() - objectA.getPosition(); // On trace une direction entre les deux objets
		objectA.move(playerToObject * -0.5f * deltaTime * speed); // On pousse les objets dans des sens opposés
		objectB.move(playerToObject * 0.5f * deltaTime * speed);
	}
}

// Gestion des collisions Joueur - Balles
void CheckPlayerBulletCollision(Player& player, Game &game)
{
	for(auto pew = game.enemyBullet.begin(); pew != game.enemyBullet.end();) // Pour chaque balle de la liste...
	{
		float D = pyth(player.shape.getPosition().x - pew->shape.getPosition().x, player.shape.getPosition().y - pew->shape.getPosition().y); // On prend la distance entre le joueur et la balle

		if(D <= player.shape.getRadius()) // Si la distance est inférieur au rayon du joueur (donc si la balle est superposée au joueur) ...
		{
			player.isDead = true; // On dit que le joueur est mort
			SpawnPlayerParticles(player, game); // On fait apparaitre des particules
			pew = game.enemyBullet.erase(pew); // On supprime la balle de la liste et on passe à la prochaine
		}
		else
		{
			++pew; // On passe à la prochaine balle de la liste
		}
	}
}

// Check des collisions Joueur - Item
bool CheckPlayerItemCollision(Player& player, Item& item)
{
	float D = pyth(player.shape.getPosition().x - item.shape.getPosition().x, player.shape.getPosition().y - item.shape.getPosition().y); // Distance entre le joueur et l'item

	if (D <= player.shape.getRadius()) // Si le joueur et l'item se superposent...
	{
		if(item.effect == "speedUp") // Si l'effet de l'item est X...
		{
			player.playerSpeed *= 2.f; // On double la vitesse du joueur
		}
		if(item.effect == "speedDown") // Si l'effet de l'item est X...
		{
			player.playerSpeed *= .5f; // On diminue sa vitesse de moitié
		}

		return true;
	}

	return false;
}

// Check des collisions Ennemi - Balles
bool CheckEnemyBulletCollision(sf::CircleShape& enemy, Game &game)
{
	for(auto pew = game.bullets.begin(); pew != game.bullets.end();) // Pour chaque balle de la liste...
	{
		float D = pyth(enemy.getPosition().x - pew->shape.getPosition().x, enemy.getPosition().y - pew->shape.getPosition().y); // Distance entre la balle et l'ennemi

		if(D <= enemy.getRadius()) // Si la balle et l'ennemi se superposent...
		{
			pew = game.bullets.erase(pew); // On supprime la balle et on répond true
			return true;
		}

		++pew; // On passe à la prochaine balle
	}

	return false;
}

// Gestion de toutes les collisions
void CheckAllTheCollisions(Player& player, Game& game, sf::FloatRect boundingBoxes[4])
{
	CheckPlayerWallCollision(player.shape, boundingBoxes, player.playerSpeed);
	CheckPlayerBulletCollision(player, game);

	for(auto it = game.enemies.begin(); it != game.enemies.end();) // Pour chaque ennemi...
	{
		CheckCollision(player.shape, it->shape, game.deltaTime.asSeconds());
		CheckEnemyWallCollision(*it, boundingBoxes);
		CheckPlayerWallCollision(it->shape, boundingBoxes, it->enemySpeed); // Cette fonction evite que les ennemis qui spawnent dans le mur restent coincés dedans

		if(CheckEnemyBulletCollision(it->shape, game)) // Si l'ennemi se fait toucher par une balle...
		{
			SpawnParticles(*it, game); // On fait apparaitre des particules
			it = game.enemies.erase(it); // On supprime l'ennemi de la liste et on passe au prochain
		}
		else
		{
			++it; // On passe au prochain ennemi
		}
	}

	for(auto it = game.bullets.begin(); it != game.bullets.end();) // Pour chaque balle...
	{
		if (CheckBulletWallCollision(it->shape, boundingBoxes)) // Si la balle touche un mur...
		{
			it = game.bullets.erase(it); // Supprime la balle et pas à la prochaine
		}
		else
		{
			++it; // Passe à la prochaine balle
		}
	}

	for (auto it = game.enemyBullet.begin(); it != game.enemyBullet.end();) // Pour chaque balle ennemie...
	{
		if (CheckBulletWallCollision(it->shape, boundingBoxes)) // Si la balle touche un mur...
		{
			it = game.enemyBullet.erase(it); // Supprime la balle et pas à la prochaine
		}
		else
		{
			++it; // Passe à la prochaine balle
		}
	}

	for (auto it = game.items.begin(); it != game.items.end();) // Pour chaque item...
	{
		CheckItemWallCollision(it->shape, boundingBoxes);

		if(CheckPlayerItemCollision(player, *it)) // Si le joueur touche un item...
		{
			it = game.items.erase(it); // Supprime l'item et passe au prochain
		}
		else
		{
			++it; // Passe au prochain item
		}
	}
}

// Generation d'une direction aleatoire
sf::Vector2f RandomDirection()
{
	int tab[2]{ -1, 1 }; // Petit tableau comportant un négatif et un positif
	sf::Vector2f direction((tab[rand() % 2]), (tab[rand() % 2])); // Vecteur de direction qui peut valoir (1,1), (-1,1), (1,-1), (-1,-1)
	return direction; // Répond la direction
}

// Fonctions de mouvement
void MoveEnemies(Enemy& enemy, float deltaTime)
{
	enemy.shape.move(enemy.direction * enemy.enemySpeed * deltaTime);
}

void MoveEnemyBullets(EnemyBullet& enemyBullet, float deltaTime)
{
	enemyBullet.shape.move(enemyBullet.direction * enemyBullet.bulletSpeed * deltaTime);
}

void MoveBullets(Bullet& bullet, float deltaTime)
{
	bullet.shape.move(bullet.direction * bullet.bulletSpeed * deltaTime);
}

void MoveParticles(Particles& particle, float deltaTime)
{
	particle.shape.move(particle.direction * particle.particleSpeed * deltaTime);
}

void MoveBoss(Boss& boss, sf::CircleShape& player, float deltaTime)
{
	boss.direction = player.getPosition() - boss.shape.getPosition();
	float amplitude = sqrtf(boss.direction.x * boss.direction.x + boss.direction.y * boss.direction.y); // longueur du vecteur
	boss.direction = boss.direction / amplitude; // Normalisation du vecteur
	boss.shape.move(boss.direction * boss.bossSpeed * deltaTime);
}

void RotateShield(Boss& boss, BossShield& shield, float deltaTime)
{
	shield.shape.move(shield.direction * shield.speed * deltaTime);
	sf::Vector2f bossToShield = shield.shape.getPosition() - boss.shape.getPosition();
	shield.direction = sf::Vector2f(-bossToShield.y, bossToShield.x);
	shield.direction = -shield.direction / pyth(shield.direction.x, shield.direction.y);
}

void MoveCACs(BossCAC& cac, Boss& boss)
{
	cac.shape.setPosition(boss.shape.getPosition() + (cac.direction * 60.f));
}