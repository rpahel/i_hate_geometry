#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>
#include <unordered_map>
#include <time.h>
#include <cmath>
#include "windows.h"
#include "maths.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "item.h"

std::string getAppPath();
std::string getAssetsPath(std::string appPath);
void levelTextUpdater(int& currentlevel, sf::Text &text);

int main()
{
	srand(time(NULL)); //On laisse ça là pour les random

	sf::RenderWindow window(sf::VideoMode(1200, 900), "I hate geometry");
	window.setVerticalSyncEnabled(true);

	int thickness = 20; // Largeur des murs;

	sf::RectangleShape wallSouth(sf::Vector2f(1200, thickness));
	wallSouth.setFillColor(sf::Color(100,100,100));
	wallSouth.setPosition(0, 900 - thickness);

	sf::RectangleShape wallNorth(sf::Vector2f(1200, thickness));
	wallNorth.setFillColor(sf::Color(100, 100, 100));

	sf::RectangleShape wallEast(sf::Vector2f(thickness, 900));
	wallEast.setFillColor(sf::Color(100, 100, 100));
	wallEast.setPosition(1200 - thickness, 0);

	sf::RectangleShape wallWest(sf::Vector2f(thickness, 900));
	wallWest.setFillColor(sf::Color(100, 100, 100));

	// On prend les hitbox des murs
	sf::FloatRect boundingBoxes[4] {wallSouth.getGlobalBounds(), wallNorth.getGlobalBounds(), wallEast.getGlobalBounds(), wallWest.getGlobalBounds()};

	int currentlevel = 1;
	bool isLoadingRoom = false;
	bool isNewRoom = true; // Est-ce que c'est une nouvelle pièce ? Début = oui
	bool firstFrame = true; //Est-ce que c'est la première frame de la salle ? Début = oui

	sf::Font font;		//Setup de la font et des différents textes
	font.loadFromFile(getAssetsPath(getAppPath()) + "arial.ttf");
	sf::Text text;
	text.setFont(font);
	text.setPosition(1080,50);
	text.setCharacterSize(24);

	sf::Clock clock;
	sf::Clock clock2;

	sf::CircleShape player;
	bool isDead = false;

	std::list<Enemy> enemies;	  //Listes des enemies et des différentes bullet
	std::list<Bullet> bullets;
	std::list<EnemyBullet> enemyBullet;
	std::list<Item> items;
	std::list<Particles> particles;

	sf::Vector2f mousePos;
	float timeSinceLastFire = 0; // Calculer la durée depuis le dernier tir
	float nextFireTime = .2f; // Durée avant de pouvoir tirer;
	
	int numberOfEnemies = 5; //Nombre d'ennemis à la première salle
	int numberOfItem = 2;
	float moveDuration = 0; // Calculer la durée de déplacement des ennemis
	float shootDuration = 0;

	float playerSpeed = 300.f;

	while (window.isOpen())
	{
		// Inputs
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;

			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::A) //Si on appuie sur A, load une nouvelle room
				{
					isLoadingRoom = true;
					enemies.clear();
					bullets.clear();
					enemyBullet.clear();
					items.clear();
					player.setPosition(600, 450);
					isNewRoom = true;
					isLoadingRoom = false;
				}
				break;

			default:
				break;
			}
		}

		// Quand c'est une nouvelle salle, on initialise le joueur et les ennemis.
		if(isNewRoom)
		{
			player = SpawnPlayer();
			SpawnEnemies(enemies, (numberOfEnemies + currentlevel) - 1, thickness);
			SpawnItems(items, numberOfItem, thickness);
			currentlevel++;
			isNewRoom = false;
		}
		levelTextUpdater(currentlevel, text); //Update le text du level

		// Logique
		sf::Time elapsedTime = clock.restart(); // Calcul du temps ecoule depuis la derniere boucle
		timeSinceLastFire += elapsedTime.asSeconds(); // Calcul du temps depuis le début le lancement du programme

		moveDuration += elapsedTime.asSeconds(); // On rajoute le deltaTime à moveDuration
		shootDuration += elapsedTime.asSeconds();

		PlayerMovement(player, playerSpeed, elapsedTime.asSeconds());

		if(moveDuration > 1.f) //Cooldown changement de direction des enemy
		{
			for (auto it = enemies.begin(); it != enemies.end(); ++it)
			{
				ChangeEnemyDirection(it->direction);
			}
			moveDuration = 0.f;
		}

		for (auto it = enemies.begin(); it != enemies.end(); ++it)
		{
			MoveEnemies(it->shape, it->direction, elapsedTime.asSeconds());
		}

		for (auto it = bullets.begin(); it != bullets.end(); ++it)
		{
			MoveBullets(it->shape, it->direction, it->rotation, elapsedTime.asSeconds());
		}

		for (auto it = enemyBullets.begin(); it != enemyBullets.end(); ++it)
		{
			MoveEnemyBullets(it->shape, it->direction, it->rotation, elapsedTime.asSeconds());
		}

		//FireBullets
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isDead)
		{
			if (timeSinceLastFire >= nextFireTime)
			{
				timeSinceLastFire = 0;
				float radians = std::atan2(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
				mousePos = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
				SpawnBullet(bullets, player, mousePos, thickness, radians);
			}
		}
			
		if (shootDuration > 3.f)
		{
			for (auto it = enemies.begin(); it != enemies.end(); ++it)
			{
				SpawnEnemiesBullet(enemyBullet, *it, player, thickness);
			}
			shootDuration = 0.f;
		}

		CheckAllTheCollisions(player, enemies, boundingBoxes, bullets, enemyBullets, isDead, elapsedTime.asSeconds()); // On check toutes les collisions (sauf entre les enemies)

		// Rendu
		window.clear();

		for(auto it = enemies.begin(); it != enemies.end(); ++it)
		{
			window.draw(it->shape);
		}

		for (auto it = bullets.begin(); it != bullets.end(); ++it)
		{
			window.draw(it->shape);
		}

		for (auto it = enemyBullets.begin(); it != enemyBullets.end(); ++it)
		{
			window.draw(it->shape);
		}

		for (auto it = items.begin(); it != items.end(); ++it)
		{
			window.draw(it->shape);
		}

		if (isLoadingRoom == false)
		{
			if(!isDead)
			{
				window.draw(player);
			}
			window.draw(wallNorth);
			window.draw(wallEast);
			window.draw(wallSouth);
			window.draw(wallWest);
		}
		window.draw(text);

		window.display();
	}
}



std::string getAppPath()
{
	char cExeFilePath[256];
	GetModuleFileNameA(NULL, cExeFilePath, 256);
	std::string exeFilePath = cExeFilePath;
	int exeNamePos = exeFilePath.find_last_of("\\/");
	std::string appPath = exeFilePath.substr(0, exeNamePos + 1);
	return appPath;
}

std::string getAssetsPath(std::string appPath)
{
	std::string assetsPath = appPath + "Assets\\";
	return assetsPath;
}

void levelTextUpdater(int& currentlevel, sf::Text& text)
{
	text.setString("level " + std::to_string(currentlevel - 1));
}
