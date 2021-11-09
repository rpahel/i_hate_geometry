#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>
#include <unordered_map>
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

	sf::CircleShape player;

	std::list<sf::CircleShape> enemies;
	std::list<sf::RectangleShape> bullets;
	bool canFire = true;
	
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
		moveDuration += elapsedTime.asSeconds(); // On rajoute le deltaTime à moveDuration

		PlayerMovement(player, elapsedTime.asSeconds());

		//FireBullets
		if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			SpawnBullet(bullets, player);
		}

		CheckAllTheCollisions(player, enemies, boundingBoxes, elapsedTime.asSeconds()); // On check toutes les collisions (sauf entre les enemies)

		for (auto it = enemies.begin(); it != enemies.end(); ++it)
		{
			//MoveEnemies(*it, enemiesDirections[*it], elapsedTime.asSeconds());
		}

		// Rendu
		window.clear();

		for(auto it = enemies.begin(); it != enemies.end(); ++it)
		{
			window.draw(*it);
		}

		for (auto it = bullets.begin(); it != bullets.end(); ++it)
		{
			window.draw(*it);
		}

		window.draw(player);
		window.draw(wallNorth);
		window.draw(wallEast);
		window.draw(wallSouth);
		window.draw(wallWest);

		window.display();
	}
}