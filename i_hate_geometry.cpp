#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>
#include <unordered_map>
#include <time.h>
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

	bool isNewRoom = true; // Est-ce que c'est une nouvelle pièce ? Début = oui
	bool firstFrame = true; //Est-ce que c'est la première frame de la salle ? Début = oui

	sf::Clock clock;
	sf::Clock clock2;

	sf::CircleShape player;

	std::list<Enemy> enemies;
	std::list<Bullet> bullets;

	sf::Vector2f mousePos;
	float fireRate = 4.0f;
	float nextFireTime = 0.0f;
	
	int numberOfEnemies = 10; //Nombre d'ennemis à la première salle
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
			SpawnEnemies(enemies, numberOfEnemies, thickness);
			isNewRoom = false;
		}

		// Logique
		sf::Time elapsedTime = clock.restart(); // Calcul du temps ecoule depuis la derniere boucle
		sf::Time time = clock2.getElapsedTime(); // Calcul du temps depuis le début le lancement du programme

		moveDuration += elapsedTime.asSeconds(); // On rajoute le deltaTime à moveDuration

		PlayerMovement(player, elapsedTime.asSeconds());



		for (auto it = enemies.begin(); it != enemies.end(); ++it)
		{
			MoveEnemies(it->shape, it->direction, elapsedTime.asSeconds());
		}

		for (auto it = bullets.begin(); it != bullets.end(); ++it)
		{
			MoveBullets(it->shape, it->direction, elapsedTime.asSeconds());
		}

		//FireBullets
		if(time.asSeconds() >= nextFireTime)
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				mousePos = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
				SpawnBullet(bullets, player, mousePos, thickness);
				nextFireTime = time.asSeconds() + 1.0f / fireRate;
			}
		}

		CheckAllTheCollisions(player, enemies, boundingBoxes, elapsedTime.asSeconds()); // On check toutes les collisions (sauf entre les enemies)

		// Rendu
		window.clear();

		for(auto it = enemies.begin(); it != enemies.end(); ++it)
		{
			//std::cout << it->name << ": " << it->shape.getPosition().x << ", " << it->shape.getPosition().y << std::endl;
			window.draw(it->shape);
		}

		for (auto it = bullets.begin(); it != bullets.end(); ++it)
		{
			window.draw(it->shape);
		}

		window.draw(player);
		window.draw(wallNorth);
		window.draw(wallEast);
		window.draw(wallSouth);
		window.draw(wallWest);

		window.display();
	}
}