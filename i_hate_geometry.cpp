#include <iostream>
#include <SFML/Graphics.hpp>
#include "maths.h"
#include "player.h"
#include "enemy.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1200, 900), "I hate geometry");
	window.setVerticalSyncEnabled(true);

	sf::CircleShape player(50);
	player.setPosition(sf::Vector2f(200.f, 400.f));

	sf::RectangleShape rectangle = RandomEnemySpawn();
	sf::CircleShape ennemy(50);
	ennemy.setFillColor(sf::Color::Red);
	ennemy.setPosition(sf::Vector2f(800.f, 400.f));
	sf::Clock clock;

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

		// Logique
		sf::Time elapsedTime = clock.restart(); // Calcul du temps �coul� depuis la derni�re boucle

		PlayerMovement( player, elapsedTime.asSeconds());
		//BiggerCheckCollisions(player, ennemies, elapsedTime.asSeconds()); // Faire un CheckCollision pour chaque ennemy de la liste ennemies;
		CheckCollision(player, ennemy, elapsedTime.asSeconds());

		// Rendu
		window.clear();
		window.draw(rectangle);

		window.draw(ennemy);
		window.draw(player);

		window.display();
	}
}