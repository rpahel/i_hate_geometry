#include <iostream>
#include <SFML/Graphics.hpp>
#include "bullet.h"

void SpawnBullet(std::list<Bullet>& bullets, sf::CircleShape& player, sf::Vector2f mousePos, int thickness, float radians)
{
	Bullet bullet;
	bullet.shape.setSize(sf::Vector2f(3, 20));
	bullet.shape.setOrigin(bullet.shape.getSize().x / 2, bullet.shape.getSize().y / 2); // change l'origine du rectangle pour être au centre
	bullet.name = "bullet" + std::to_string(bullets.size());
	bullet.direction = mousePos - player.getPosition();
	float amplitude = sqrtf(bullet.direction.x * bullet.direction.x + bullet.direction.y * bullet.direction.y); // longueur du vecteur
	bullet.direction = bullet.direction / amplitude; // Normalisation du vecteur
	bullet.rotation = std::atan2(bullet.direction.x, bullet.direction.y); // en radian
	bullet.rotation = - bullet.rotation * (180.f / 3.1415f); // Conversion en deg
	bullet.shape.setRotation(bullet.rotation);
	bullet.shape.setPosition(player.getPosition().x, player.getPosition().y); // La balle sort du centre du cercle
	bullet.shape.setFillColor(sf::Color::Transparent);
	bullet.shape.setOutlineThickness(2.f);
	bullet.shape.setOutlineColor(sf::Color::Green);
	bullets.push_back(bullet);
}