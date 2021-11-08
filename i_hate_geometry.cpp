#include <iostream>
#include <SFML/Graphics.hpp>
#include "maths.h"
#include "player.h"
#include "enemy.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1200, 900), "I hate geometry");
	window.setVerticalSyncEnabled(true);

	//Player
	sf::CircleShape circle(50);
	circle.setPosition(sf::Vector2f(400.f, 300.f));

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
		sf::Time elapsedTime = clock.restart(); //< Calcul du temps écoulé depuis la dernière boucle

		PlayerMovement(circle, elapsedTime.asSeconds());
	

		// Rendu
		window.clear();
		window.draw(circle);
		window.draw(RandomEnemySpawn());
		window.display();
	}
}