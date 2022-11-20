#include "CellFocus.hpp"

void CellFocus::update(sf::IntRect _rect) {
	if (_rect != rect) {
		rect = _rect;
		if (rect.width == 0) {
			sprite.setOutlineColor(sf::Color(0, 0, 0, 0));
		} else {
			sprite.setOutlineColor(sf::Color(255, 0, 0));
			sprite.setSize(sf::Vector2f(rect.width, rect.height));
			sprite.setPosition(sf::Vector2f(rect.left, rect.top));
			sprite.setFillColor(sf::Color(0, 0, 0, 0));
		}
	}
	
}

sf::RectangleShape CellFocus::getSprite() {
	return sprite;
}