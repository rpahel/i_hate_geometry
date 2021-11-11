#include <iostream>
#include <SFML/Graphics.hpp>
#include "struct.h"
#include "maths.h"


Player SpawnPlayer()
{
	Player player;

	player.radius = 15.f; // Rayon du cercle

	// D�but propri�t�s de la forme du joueur
	player.shape.setRadius(player.radius);
	player.shape.setOrigin(player.shape.getRadius(), player.shape.getRadius());
	player.shape.setFillColor(sf::Color::Transparent);
	player.shape.setOutlineThickness(2);
	player.shape.setOutlineColor(sf::Color::Cyan);
	player.shape.setPosition(600, 450);
	// Fin propri�t�s de la forme du joueur

	player.fireRate = .25f; // Cadence de tir

	player.fireCD = player.fireRate; // Cooldown du tir

	player.playerSpeed = 350.f; // Vitesse du joueur

	player.numberOfBullets = 1; // Nombre de balles que le joueur tire par clic

	player.isDead = false; // Est-ce que le joueur est mort ?

	return player;
}

void SpawnEnemies(Game& game, int wallThickness)
{
	game.enemies.resize((game.numberOfEnemies + game.currentLevel) - 1); // On g�n�re une liste d'ennemis de taille (nombre d'ennemis au premier niveau + niveau - 1)


	int i = 0; // Chiffre utile pour nommer l'ennemi
	for (auto& enemy : game.enemies)
	{
		int enemyType = rand() % 2; // chiffre entre 0 et X-1 qui va determiner le type de l'ennemi

		switch (enemyType) // On g�n�re un type d'ennemi en fonction de enemyType
		{
			case 0:
				enemy.shape.setRadius(15.f);
				enemy.shape.setOutlineThickness(2.f);
				enemy.shape.setOutlineColor(sf::Color(255, 0, 255)); // Magenta
				enemy.type = enemyType;
				enemy.enemySpeed = 300.f;
				enemy.fireRate = 1.f;
				break;

			case 1:
				enemy.shape.setRadius(20.f);
				enemy.shape.setOutlineThickness(2.f);
				enemy.shape.setOutlineColor(sf::Color(255, 165, 0)); // Orange
				enemy.type = enemyType;
				enemy.enemySpeed = 250.f;
				enemy.fireRate = 1.2f;
				break;

			default:
				enemy.shape.setRadius(15.f);
				enemy.shape.setOutlineThickness(2.f);
				enemy.shape.setOutlineColor(sf::Color(255, 0, 255)); // Magenta
				enemy.type = 0;
				enemy.enemySpeed = 300.f;
				enemy.fireRate = 1;
				break;
		}

		enemy.fireCD = enemy.fireRate; // Cooldown du tir

		enemy.shape.setFillColor(sf::Color::Transparent); // On rend sa couleur de remplissage transparente
		enemy.shape.setOrigin(enemy.shape.getRadius(), enemy.shape.getRadius()); // On mets son point d'origine au centre de la forme
		enemy.shape.setPosition(rand() % (1200 - wallThickness * 2) + wallThickness, rand() % (900 - wallThickness * 2) + wallThickness); // On le fait appara�tre � un endroit au hasard � l'int�rieur des murs

		enemy.name = "enemy_" + std::to_string(i) + "_" + "type_" + std::to_string(enemy.type); // Nom de l'ennemi

		enemy.direction = RandomDirection(); // On lui donne une direction de d�part al�atoire

		enemy.moveDuration = rand() % 3 + 1; // On lui assigne une dur�e de d�placement entre 1 et 3 secondes
		enemy.moveCD = enemy.moveDuration;

		++i;
	}
}

void SpawnEnemiesBullet(Game& game, Enemy& enemy, sf::CircleShape& player, int bulletNumber)
{
	EnemyBullet enemyBullet;

	enemyBullet.shape.setSize(sf::Vector2f(2.f, 15.f)); // On d�finit la taille de la balle
	enemyBullet.shape.setOrigin(enemyBullet.shape.getSize().x / 2, enemyBullet.shape.getSize().y / 2); // On change l'origine du rectangle pour �tre au centre de la forme

	enemyBullet.name = "enemyBullet" + std::to_string(game.enemyBullet.size());

	enemyBullet.direction = player.getPosition() - enemy.shape.getPosition();
	float amplitude = sqrtf(enemyBullet.direction.x * enemyBullet.direction.x + enemyBullet.direction.y * enemyBullet.direction.y); // longueur du vecteur
	enemyBullet.direction = enemyBullet.direction / amplitude; // Normalisation du vecteur

	switch (bulletNumber)
	{
		case 0:
			break;

		case 1:
			enemyBullet.direction = -enemyBullet.direction;
			break;

		default:
			break;
	}

	enemyBullet.rotation = std::atan2(enemyBullet.direction.x, enemyBullet.direction.y); // en radian
	enemyBullet.rotation = -enemyBullet.rotation * (180.f / 3.1415f); // Conversion en deg
	enemyBullet.shape.setRotation(enemyBullet.rotation); // La balle est tourn�e en direction de.. sa direction
	enemyBullet.shape.setPosition(enemy.shape.getPosition()); // La balle sort du centre du cercle
	enemyBullet.shape.setFillColor(sf::Color::Transparent); // La couleur de la balle
	enemyBullet.shape.setOutlineThickness(2.f); // L'�paisseur des contours de la balle
	enemyBullet.shape.setOutlineColor(sf::Color::Yellow); // Couleur des contours de la balle

	enemyBullet.bulletSpeed = 600.f;

	game.enemyBullet.push_back(enemyBullet); // On rajoute la balle � la liste
}

void SpawnBullet(Game& game, const Player& player, const Mouse& mouse)
{
	Bullet bullet;

	bullet.name = "bullet" + std::to_string(game.bullets.size()); // Nom de la balle

	bullet.direction = sf::Vector2f(mouse.x, mouse.y) - player.shape.getPosition(); // Creation d'un vecteur en direction de la souris
	float amplitude = sqrtf(bullet.direction.x * bullet.direction.x + bullet.direction.y * bullet.direction.y); // longueur du vecteur
	bullet.direction = bullet.direction / amplitude; // Normalisation du vecteur

	bullet.rotation = std::atan2(bullet.direction.x, bullet.direction.y); // en radian
	bullet.rotation = -bullet.rotation * (180.f / 3.1415f); // Conversion en deg

	bullet.shape.setSize(sf::Vector2f(2, 15)); // On d�finit la taille de la balle
	bullet.shape.setOrigin(bullet.shape.getSize().x / 2, bullet.shape.getSize().y / 2); // On change l'origine du rectangle pour �tre au centre de la forme
	bullet.shape.setRotation(bullet.rotation); // La balle est tourn�e en direction de.. sa direction
	bullet.shape.setPosition(player.shape.getPosition().x, player.shape.getPosition().y); // La balle sort du centre du cercle
	bullet.shape.setFillColor(sf::Color::Transparent); // La couleur de la balle
	bullet.shape.setOutlineThickness(2.f); // L'�paisseur des contours de la balle
	bullet.shape.setOutlineColor(sf::Color::Green); // Couleur des contours de la balle

	bullet.bulletSpeed = 800.f;

	game.bullets.push_back(bullet);
}

void SpawnItems(Game& game, int wallThickness)
{
	game.items.resize(game.numberOfItems); // On g�n�re une liste d'items

	int i = 0;
	for (auto& item : game.items)
	{
		item.name = "item" + std::to_string(i); // Nom de l'item

		item.shape.setSize(sf::Vector2f(10.f, 10.f)); // Taille de l'item
		item.shape.setOrigin(item.shape.getSize().x / 2, item.shape.getSize().y / 2); // On met son origine au centre de la forme
		item.shape.setPosition(rand() % (1200 - wallThickness * 2) + wallThickness, rand() % (900 - wallThickness * 2) + wallThickness); // On le mets al�atoirement � l'int�rieur des murs
		item.shape.setFillColor(sf::Color::Transparent); // Couleur de la forme
		item.shape.setOutlineThickness(2.f); // Epaisseur des contours de la forme

		int itemType = rand() % 2; // Chiffre entre 0 et X-1 qui va determiner le type d'item
 		switch (itemType)
		{
			case 0:
				item.shape.setOutlineColor(sf::Color::White);
				item.effect = "speedUp";
				break;

			case 1:
				item.shape.setOutlineColor(sf::Color::Red);
				item.effect = "speedDown";
				break;

			default:
				item.shape.setOutlineColor(sf::Color::White);
				item.effect = "speedUp";
				break;
		}

		++i;
	}
}

void SpawnParticles(const Enemy& enemy, Game& game)
{
	int numberOfParticles = 20; // Nombre de particules que lache un ennemi a sa mort

	for (int i = 0; i < numberOfParticles; i++)
	{
		float lifeTime = ((rand() % 301) / 1000.f) + 0.2f; // On donne une dur�e de vie � la particule (entre 0.2f et 0.5f secondes)
		float rot = (i * 2.f) * 3.14f / numberOfParticles; // angle de la direction dans laquelle la particule se dirigera
		sf::Vector2f direction(cos(rot), sin(rot)); // direction de la particule

		Particles particle;

		particle.name = "particle" + std::to_string(game.particles.size()); // On nomme la particule

		particle.direction = direction; // On assigne la direction � la particule

		particle.rotation = std::atan2(particle.direction.x, particle.direction.y); // en radian
		particle.rotation = -particle.rotation * (180.f / 3.1415f); // Conversion en deg

		particle.shape.setSize(sf::Vector2f(1.f, 5.f)); // Taille de la particule
		particle.shape.setOrigin(particle.shape.getSize().x / 2, particle.shape.getSize().y / 2); // On change l'origine du rectangle pour �tre au centre
		particle.shape.setRotation(particle.rotation); // On dirige la particule en direction de sa direction
		particle.shape.setPosition(enemy.shape.getPosition()); // La balle sort du centre du cercle
		particle.shape.setFillColor(sf::Color::Transparent); // Couleur de la particule
		particle.shape.setOutlineThickness(1.f); // Epaisseur des contours de la forme
		particle.shape.setOutlineColor(enemy.shape.getOutlineColor()); // Couleur des contours de la forme

		particle.lifeTime = lifeTime; // On assigne la dur�e de vie

		particle.particleSpeed = rand() % 200 + 400;
		std::cout << particle.particleSpeed << std::endl;

		game.particles.push_back(particle); // On rajoute la particule � la liste
	}
}