#include "Button.hpp"

void Button::display(sf::RenderWindow* window) {
	window->draw(sprite);
}

bool Button::interact(sf::Vector2i mouse, bool clic) {
	if (pointInFrame(sf::Vector2f(mouse.x, mouse.y), sprite.getGlobalBounds())) sprite.setScale(1.05, 1.05);
	else sprite.setScale(1, 1);
	nextClicState(clicState, clic, pointInFrame(sf::Vector2f(mouse.x, mouse.y), sprite.getGlobalBounds()));
	return (clicState == 3);
}

std::string Button::getName() {
	return name;
}