#pragma once
#include "commons.hpp"


class RectByCenter
{
public:
	sf::Vector2f center; sf::Vector2f radius;
	RectByCenter() {
		center = sf::Vector2f(0, 0);
		radius = sf::Vector2f(1, 1);
	}

	RectByCenter(sf::Vector2f c, sf::Vector2f r) {
		center = c;
		radius = r;
	}
};