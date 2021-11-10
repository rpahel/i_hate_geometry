#include <iostream>
#include <SFML/Graphics.hpp>
#include "item.h"
#include "maths.h"

void SpawnItems(Game &game, int numberOfItem, int thickness)
{
	game.items.resize(numberOfItem);
	int i = 0;

	for (auto& item : game.items)
	{
		item.name = "item" + std::to_string(i);
		item.shape.setSize(sf::Vector2f(25.f, 25.f));
		item.shape.setOrigin(item.shape.getSize().x / 2, item.shape.getSize().y / 2);
		item.shape.setPosition(rand() % (1200 - thickness * 2) + thickness, rand() % (900 - thickness * 2) + thickness);
		item.shape.setFillColor(sf::Color::Transparent);
		item.shape.setOutlineThickness(2.f);
		if(rand()%2)
		{
			item.shape.setOutlineColor(sf::Color::White);
			item.effect = "speedUp";
		}
		else
		{
			item.shape.setOutlineColor(sf::Color::Red);
			item.effect = "speedDown";
		}

		++i;
	}
}