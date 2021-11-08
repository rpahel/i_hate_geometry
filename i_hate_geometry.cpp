#include <iostream>
#include <SFML/Graphics.hpp>

void RectMovement(sf::CircleShape& circle, float deltaTime);

int main()
{
	sf::RenderWindow window(sf::VideoMode(1200, 900), "Ma première fenêtre");
	window.setVerticalSyncEnabled(true);

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

		RectMovement( circle, elapsedTime.asSeconds());

		// Rendu
		window.clear();

		window.draw(circle);

		window.display();
	}
}

// Gestion des déplacement d'un rectangle
void RectMovement(sf::CircleShape& circle, float deltaTime)
{
	float speed = 300.f; // 300 pixels par seconde

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		circle.move(sf::Vector2f(0.f, -speed * deltaTime));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		circle.move(sf::Vector2f(0.f, speed * deltaTime));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		circle.move(sf::Vector2f(-speed * deltaTime, 0.f));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		circle.move(sf::Vector2f(speed * deltaTime, 0.f));
}