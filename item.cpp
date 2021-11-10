#include <iostream>
#include <SFML/Graphics.hpp>
#include "item.h"
#include "maths.h"

void SpawnItems(std::list<Item>& items, int numberOfItem, int thickness)
{
	items.resize(numberOfItem);
	int i = 0;

	for (auto& item : items)
	{
		item.name = "item" + std::to_string(i);
		item.shape.setSize(sf::Vector2f(25.f, 25.f));
		item.shape.setOrigin(item.shape.getSize());
		item.shape.setPosition(rand() % (1200 - thickness * 2) + thickness, rand() % (900 - thickness * 2) + thickness);
		item.shape.setFillColor(sf::Color::Transparent);
		item.shape.setOutlineThickness(2.f);
		item.shape.setOutlineColor(sf::Color::White);
		++i;
	}
}