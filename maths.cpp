#include <iostream>
#include <SFML/Graphics.hpp>

// Gestion des déplacement d'un player
void PlayerMovement(sf::CircleShape& circle, float deltaTime)
{
	float speed = 600.f; // 600 pixels par seconde

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		circle.move(sf::Vector2f(0.f, -speed * deltaTime));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		circle.move(sf::Vector2f(0.f, speed * deltaTime));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		circle.move(sf::Vector2f(-speed * deltaTime, 0.f));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		circle.move(sf::Vector2f(speed * deltaTime, 0.f));
}

// Gestion des collisions
void CheckCollision(sf::CircleShape& player, sf::CircleShape& object, float deltaTime)
{
	float speed = 8.f;
	// Check si les deux cercles se superposent
	if((player.getPosition().x - object.getPosition().x) * (player.getPosition().x - object.getPosition().x) + 
		(player.getPosition().y - object.getPosition().y) * (player.getPosition().y - object.getPosition().y) <= 
		(player.getRadius() + object.getRadius()) * (player.getRadius() + object.getRadius())) // dX^2 + dY^2 <= (R1 - R2)^2
	{
		// S'ils se superposent, les déplacer pour qu'ils ne se superposent plus
		sf::Vector2f playerToObject = object.getPosition() - player.getPosition();
		player.move(playerToObject * -0.5f * deltaTime * speed);
		object.move(playerToObject * 0.5f * deltaTime * speed);
	}
	// S'ils ne se superposent pas, rien faire
}