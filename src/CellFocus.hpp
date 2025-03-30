#pragma once
#include "SFML/Graphics.hpp"

class CellFocus {
	sf::RectangleShape sprite;
	sf::IntRect rect;
public:
	void update(sf::IntRect _rect);
	CellFocus() {
		sprite.setOutlineColor(sf::Color(255, 0, 0));
		sprite.setOutlineThickness(5);
		rect = sf::IntRect(0, 0, 0, 0);
	}
	sf::RectangleShape getSprite();
};