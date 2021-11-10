#include <iostream>
#include <SFML/Graphics.hpp>
#include "particle.h"
#include "maths.h"

void SpawnParticles(sf::Vector2f spawnPos, Game &game)
{
	//int lifeTime = rand() % 2 + 1;
	int numberOfParticles = 20;
	int thickness = 20;
	int i = 0;
	
	for (int i = 0; i < numberOfParticles; i++)
	{
		sf::Vector2f dir(50, 50);
		Particles particle;
		particle.shape.setSize(sf::Vector2f(3, 20));
		particle.shape.setOrigin(particle.shape.getSize().x / 2, particle.shape.getSize().y / 2); // change l'origine du rectangle pour être au centre
		particle.name = "particle" + std::to_string(game.particles.size());
		particle.direction = dir;
		float amplitude = sqrtf(particle.direction.x * particle.direction.x + particle.direction.y * particle.direction.y); // longueur du vecteur
		particle.direction = particle.direction / amplitude; // Normalisation du vecteur
		particle.rotation = std::atan2(particle.direction.x, particle.direction.y); // en radian
		particle.rotation = -particle.rotation * (180.f / 3.1415f); // Conversion en deg
		particle.shape.setRotation(particle.rotation);
		particle.shape.setPosition(spawnPos); // La balle sort du centre du cercle
		particle.shape.setFillColor(sf::Color::Transparent);
		particle.shape.setOutlineThickness(2.f);
		particle.shape.setOutlineColor(sf::Color::Blue);
		game.particles.push_back(particle);
	}
}