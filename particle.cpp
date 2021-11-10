#include <iostream>
#include <SFML/Graphics.hpp>
#include "particle.h"
#include "maths.h"

void SpawnParticles(std::list<Particles>& particles, int numberOfParticles, int thickness)
{
	particles.resize(numberOfParticles);
	int i = 0;

	for (auto& particle : particles)
	{
		particle.shape.setSize(sf::Vector2f(3, 20));
		particle.shape.setOrigin(particle.shape.getSize().x / 2, particle.shape.getSize().y / 2); // change l'origine du rectangle pour être au centre
		particle.name = "particle" + std::to_string(particles.size());
		//particle.direction = mousePos - player.getPosition();
		//float amplitude = sqrtf(particle.direction.x * particle.direction.x + bullet.direction.y * bullet.direction.y); // longueur du vecteur
		//particle.direction = bullet.direction / amplitude; // Normalisation du vecteur
		//particle.rotation = std::atan2(bullet.direction.x, bullet.direction.y); // en radian
		//particle.rotation = -bullet.rotation * (180.f / 3.1415f); // Conversion en deg
		particle.shape.setRotation(particle.rotation);
		//particle.shape.setPosition(player.getPosition().x, player.getPosition().y); // La balle sort du centre du cercle
		particle.shape.setFillColor(sf::Color::Transparent);
		particle.shape.setOutlineThickness(2.f);
		particle.shape.setOutlineColor(sf::Color::Green);
		particles.push_back(particle);
	}
}