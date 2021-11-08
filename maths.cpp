#include <iostream>
#include <SFML/Graphics.hpp>

// Gestion des déplacement d'un player
void PlayerMovement(sf::CircleShape& circle, float deltaTime)
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