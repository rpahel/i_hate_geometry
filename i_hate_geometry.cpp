#include <iostream>
#include <SFML/Graphics.hpp>
#include "maths.h"
#include "player.h"
#include "enemy.h"

int main()
{
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

	sf::Clock clock;

	sf::CircleShape player;

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
			isNewRoom = false;
		}


		// Logique
		sf::Time elapsedTime = clock.restart(); // Calcul du temps ecoule depuis la derniere boucle

		PlayerMovement(player, elapsedTime.asSeconds());
		CheckWallCollision(player, boundingBoxes, elapsedTime.asSeconds());
		//BiggerCheckCollisions(player, ennemies, elapsedTime.asSeconds()); // Faire un CheckCollision pour chaque ennemy de la liste ennemies;
		//CheckCollision(player, enemy, elapsedTime.asSeconds());

		// Rendu
		window.clear();

		//window.draw(rectangle);
		//window.draw(enemy);
		window.draw(player);
		window.draw(wallNorth);
		window.draw(wallEast);
		window.draw(wallSouth);
		window.draw(wallWest);

		window.display();
	}
}