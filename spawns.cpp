#include <iostream>
#include <SFML/Graphics.hpp>
#include "struct.h"
#include "maths.h"

Player SpawnPlayer()
{
	Player player;

	player.radius = 20.f; // Rayon du cercle

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
				enemy.shape.setRadius(20.f);
				enemy.shape.setOutlineThickness(2.f);
				enemy.shape.setOutlineColor(sf::Color(255, 0, 255)); // Magenta
				enemy.type = enemyType;
				enemy.enemySpeed = 300.f;
				enemy.fireRate = 1.f;
				break;

			case 1:
				enemy.shape.setRadius(25.f);
				enemy.shape.setOutlineThickness(2.f);
				enemy.shape.setOutlineColor(sf::Color(255, 165, 0)); // Orange
				enemy.type = enemyType;
				enemy.enemySpeed = 250.f;
				enemy.fireRate = 1.2f;
				break;

			default:
				enemy.shape.setRadius(20.f);
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

		item.shape.setSize(sf::Vector2f(20.f, 20.f)); // Taille de l'item
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

			//case 2: 
				//item qui fait tirer plus vite (bonus)
			//case 3:
				//item qui grossit les ennemis (bonus)
			//case 4:
				//item item qui slow les ennemis (bonus)
			//case 5:
				//item qui accelere les ennemis (malus)
			//case 6:
				//item qui fait tirer moins vite (malus)
		}

		++i;
	}
}

void SpawnItemText(sf::Font font, Game& game)
{
	game.itemText.setFont(game.font); // On assigne le font au texte
	game.itemText.setPosition(1080, 50); // On assigne une position au texte
	game.itemText.setCharacterSize(24); // On assigne une taille de police
	game.itemText.setString("level " + std::to_string(game.currentLevel)); //On affecte un texte au text
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

		game.particles.push_back(particle); // On rajoute la particule � la liste
	}
}

void SpawnPlayerParticles(const Player& player, Game& game)
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
		particle.shape.setPosition(player.shape.getPosition()); // La balle sort du centre du cercle
		particle.shape.setFillColor(sf::Color::Transparent); // Couleur de la particule
		particle.shape.setOutlineThickness(1.f); // Epaisseur des contours de la forme
		particle.shape.setOutlineColor(player.shape.getOutlineColor()); // Couleur des contours de la forme

		particle.lifeTime = lifeTime; // On assigne la dur�e de vie

		particle.particleSpeed = rand() % 200 + 400;

		game.particles.push_back(particle); // On rajoute la particule � la liste
	}
}

void SpawnBoss(Game& game)
{
	Boss boss;
	boss.name = "boss_" + std::to_string(game.bosses.size());
	boss.shape.setRadius(50.f);
	boss.shape.setOutlineThickness(2.f);
	boss.shape.setOutlineColor(sf::Color(58, 58, 194));
	boss.shape.setFillColor(sf::Color::Transparent); // On rend sa couleur de remplissage transparente
	boss.shape.setOrigin(boss.shape.getRadius(), boss.shape.getRadius()); // On mets son point d'origine au centre de la forme
	boss.shape.setPosition(600, 300); // On le fait appara�tre au milieu, et l�g�rement au-dessus du joueur
	boss.type = 0;
	boss.maxHealth = 50;
	boss.health = boss.maxHealth;
	boss.state = 2;
	boss.bossSpeed = 250.f;
	boss.fireRate = 1.f;
	boss.fireCD = boss.fireRate;
	boss.fireSpeed = 0;
	boss.changeStateTime = 0;
	boss.shieldsUp = false;
	boss.isFiring = false;
	boss.isMoving = false;
	boss.CacUp = false;

	boss.shapeContenerHealthBar.setSize(sf::Vector2f((float(boss.health) / float(boss.maxHealth)) * 100.f, 10.f));
	boss.shapeContenerHealthBar.setOutlineThickness(2.f);
	boss.shapeContenerHealthBar.setOutlineColor(sf::Color::White);
	boss.shapeContenerHealthBar.setFillColor(sf::Color::Transparent);
	boss.shapeContenerHealthBar.setOrigin(boss.shape.getOrigin());
	boss.shapeContenerHealthBar.setPosition(boss.shape.getPosition().x, boss.shape.getPosition().y - 80);

	boss.shapeHealthBar.setSize(sf::Vector2f((float(boss.health) / float(boss.maxHealth)) * 100.f, 10.f));
	boss.shapeHealthBar.setFillColor(sf::Color::Red);
	boss.shapeHealthBar.setOrigin(boss.shape.getOrigin());
	boss.shapeHealthBar.setPosition(boss.shape.getPosition().x, boss.shape.getPosition().y - 80);
	game.bosses.push_back(boss);
}

void SpawnBossShield(Boss& boss, Game& game)
{
	for(int i = 0; i < 8; i++)
	{
		BossShield shield;

		shield.name = "bossShield_" + std::to_string(i);

		shield.shape.setRadius(15);
		shield.shape.setOutlineThickness(2);
		shield.shape.setOrigin(shield.shape.getRadius(), shield.shape.getRadius());
		shield.shape.setFillColor(sf::Color::Transparent);
		shield.shape.setOutlineColor(sf::Color::White);

		shield.speed = 100.f;

		sf::Vector2f bossToShield;
		float quotient;
		switch (i)
		{
		case 0:
			shield.shape.setPosition(boss.shape.getPosition().x + boss.shape.getRadius() + shield.shape.getRadius() + 5, boss.shape.getPosition().y);
			break;
		case 1:
			shield.shape.setPosition(boss.shape.getPosition().x - boss.shape.getRadius() - shield.shape.getRadius() - 5, boss.shape.getPosition().y);
			break;
		case 2:
			shield.shape.setPosition(boss.shape.getPosition().x, boss.shape.getPosition().y + boss.shape.getRadius() + shield.shape.getRadius() + 5);
			break;
		case 3:
			shield.shape.setPosition(boss.shape.getPosition().x, boss.shape.getPosition().y - boss.shape.getRadius() - shield.shape.getRadius() - 5);
			break;
		case 4:
			shield.shape.setPosition(boss.shape.getPosition().x + boss.shape.getRadius() + shield.shape.getRadius() + 5, boss.shape.getPosition().y - boss.shape.getRadius() - shield.shape.getRadius() - 5);
			break;
		case 5:
			shield.shape.setPosition(boss.shape.getPosition().x - boss.shape.getRadius() - shield.shape.getRadius() - 5, boss.shape.getPosition().y + boss.shape.getRadius() + shield.shape.getRadius() + 5);
			break;
		case 6:
			shield.shape.setPosition(boss.shape.getPosition().x + boss.shape.getRadius() + shield.shape.getRadius() + 5, boss.shape.getPosition().y + boss.shape.getRadius() + shield.shape.getRadius() + 5);
			break;
		case 7:
			shield.shape.setPosition(boss.shape.getPosition().x - boss.shape.getRadius() - shield.shape.getRadius() - 5, boss.shape.getPosition().y - boss.shape.getRadius() - shield.shape.getRadius() - 5);
			break;
		default:
			shield.shape.setPosition(boss.shape.getPosition().x + boss.shape.getRadius() + shield.shape.getRadius() + 5, boss.shape.getPosition().y);
			break;
		}

		bossToShield = shield.shape.getPosition() - boss.shape.getPosition();
		shield.direction = sf::Vector2f(-bossToShield.y, bossToShield.x);
		shield.direction = shield.direction / pyth(shield.direction.x, shield.direction.y);

		game.bossShields.push_back(shield);
	}
}

void SpawnBossBullet(Game& game, Boss& boss, int numberOfBullets = 1)
{
	for (int i = 0; i < numberOfBullets; i++)
	{
		float rot = (i * 2.f) * 3.141592f / numberOfBullets; // angle de la direction dans laquelle la particule se dirigera
		sf::Vector2f direction(cos(rot), sin(rot)); // direction de la particule

		EnemyBullet enemyBullet;

		enemyBullet.shape.setSize(sf::Vector2f(2.f, 15.f)); // On d�finit la taille de la balle
		enemyBullet.shape.setOrigin(enemyBullet.shape.getSize().x / 2, enemyBullet.shape.getSize().y / 2); // On change l'origine du rectangle pour �tre au centre de la forme

		enemyBullet.name = "bossBullet_" + std::to_string(i);

		enemyBullet.direction = direction;
		enemyBullet.rotation = std::atan2(enemyBullet.direction.x, enemyBullet.direction.y); // en radian
		enemyBullet.rotation = -enemyBullet.rotation * (180.f / 3.141592f); // Conversion en deg(180.f / 3.1415f); // Conversion en deg
		enemyBullet.shape.setRotation(enemyBullet.rotation); // La balle est tourn�e en direction de.. sa direction
		enemyBullet.shape.setPosition(boss.shape.getPosition() + (direction * 60.f)); // La balle sort du contour du cercle
		enemyBullet.shape.setFillColor(sf::Color::Transparent); // La couleur de la balle
		enemyBullet.shape.setOutlineThickness(2.f); // L'�paisseur des contours de la balle
		enemyBullet.shape.setOutlineColor(sf::Color::Yellow); // Couleur des contours de la balle

		enemyBullet.bulletSpeed = 135.0f;

		game.enemyBullet.push_back(enemyBullet); // On rajoute la balle � la liste
	}
}

void SpawnBossCAC(Game& game, Boss& boss, int numberOfBossCACs = 1)
{
	for (int i = 0; i < numberOfBossCACs; i++)
	{
		float rot = (i * 2.f) * 3.141592f / numberOfBossCACs; // angle de la direction dans laquelle la particule se dirigera
		sf::Vector2f direction(cos(rot), sin(rot)); // direction de la particule

		BossCAC bossCAC;

		bossCAC.shape.setSize(sf::Vector2f(2.f, 15.f)); // On d�finit la taille de la balle
		bossCAC.shape.setOrigin(bossCAC.shape.getSize().x / 2, bossCAC.shape.getSize().y / 2); // On change l'origine du rectangle pour �tre au centre de la forme

		bossCAC.name = "bossCAC_" + std::to_string(i);

		bossCAC.direction = direction;
		bossCAC.rotation = std::atan2(bossCAC.direction.x, bossCAC.direction.y); // en radian
		bossCAC.rotation = -bossCAC.rotation * (180.f / 3.141592f); // Conversion en deg(180.f / 3.1415f); // Conversion en deg
		bossCAC.shape.setRotation(bossCAC.rotation); // La balle est tourn�e en direction de.. sa direction
		bossCAC.shape.setPosition(boss.shape.getPosition() + (direction * 60.f)); // La balle sort du contour du cercle
		bossCAC.shape.setFillColor(sf::Color::Transparent); // La couleur de la balle
		bossCAC.shape.setOutlineThickness(2.f); // L'�paisseur des contours de la balle
		bossCAC.shape.setOutlineColor(sf::Color::Red); // Couleur des contours de la balle

		game.bossCacs.push_back(bossCAC); // On rajoute la balle � la liste
	}
}

void LoadLevel(Game& game, Player& player, int level)
{
	game.enemies.clear();
	game.bosses.clear();
	game.bullets.clear();
	game.enemyBullet.clear();
	game.items.clear();
	game.particles.clear();
	game.bosses.clear();
	game.bossShields.clear();
	game.bossCacs.clear();
	player.playerSpeed = 300.f;
	player.shape.setPosition(600, 450);
	game.currentLevel = level;
	game.levelText.setString("level " + std::to_string(game.currentLevel));
	game.timeSinceStartLevel = 0;
	game.isNewRoom = true;
}