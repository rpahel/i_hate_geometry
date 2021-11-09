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

	int level = 0;
	bool isLoadingRoom = false;
	bool isNewRoom = true; // Est-ce que c'est une nouvelle pièce ? Début = oui
	bool firstFrame = true; //Est-ce que c'est la première frame de la salle ? Début = oui

	sf::Clock clock;
	sf::Clock clock2;

	sf::CircleShape player;	

	std::list<Enemy> enemies;	  //Listes des enemies et des différentes bullet
	std::list<Bullet> bullets;
	std::list<EnemyBullet> enemyBullet;

	sf::Vector2f mousePos;
	float timeSinceLastFire = 0; // Calculer la durée depuis le dernier tir
	float nextFireTime = 0.2f; // Durée avant de pouvoir tirer;
	
	int numberOfEnemies = 5; //Nombre d'ennemis à la première salle
	float moveDuration = 0; // Calculer la durée de déplacement des ennemis

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

			default:
				break;
			}
		}

		// Quand c'est une nouvelle salle, on initialise le joueur et les ennemis.
		if(isNewRoom)
		{
			player = SpawnPlayer();
			SpawnEnemies(enemies, numberOfEnemies + level, thickness);
			level++;
			isNewRoom = false;
		}


		//Change de salle lorsque tout les ennemies sont dead
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			isLoadingRoom = true;
			enemies.clear();
			bullets.clear();
			player.setPosition(600, 450);
			isNewRoom = true;
			isLoadingRoom = false;
		}

		// Logique
		sf::Time elapsedTime = clock.restart(); // Calcul du temps ecoule depuis la derniere boucle
		timeSinceLastFire += elapsedTime.asSeconds(); // Calcul du temps depuis le début le lancement du programme

		moveDuration += elapsedTime.asSeconds(); // On rajoute le deltaTime à moveDuration

		PlayerMovement(player, elapsedTime.asSeconds());
		GetEnemyPosition(enemies);

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

		for (auto it = enemyBullet.begin(); it != enemyBullet.end(); ++it)
		{
			MoveEnemyBullets(it->shape, it->direction, it->rotation, elapsedTime.asSeconds());
		}

		//FireBullets
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (timeSinceLastFire >= nextFireTime)
			{
				float radians = std::atan2(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
				mousePos = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
				SpawnBullet(bullets, player, mousePos, thickness, radians);
				timeSinceLastFire = 0;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			for (auto it = enemies.begin(); it != enemies.end(); ++it)
			{
				SpawnEnemiesBullet(enemyBullet, enemies, player, thickness);
			}
		}

		CheckAllTheCollisions(player, enemies, boundingBoxes, elapsedTime.asSeconds()); // On check toutes les collisions (sauf entre les enemies)

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

		for (auto it = enemyBullet.begin(); it != enemyBullet.end(); ++it)
		{
			window.draw(it->shape);
		}

		std::cout << enemyBullet.size() << std::endl;

		if (isLoadingRoom == false)
		{
			window.draw(player);
			window.draw(wallNorth);
			window.draw(wallEast);
			window.draw(wallSouth);
			window.draw(wallWest);
		}

		window.display();
	}
}