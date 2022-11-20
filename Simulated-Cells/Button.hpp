#pragma once
#include "commons.hpp"

class Button {
protected:
	sf::Sprite sprite;
	sf::Texture texture;
	std::string name;
	int clicState;
public:
	Button(std::string _name, sf::Vector2f position) {
		name = _name;
		sf::Image image;
		image.loadFromFile("../img/buttons/"+name+".png");
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setPosition(position);
		sf::FloatRect dimensions = sprite.getGlobalBounds();
		sprite.setOrigin(dimensions.width / 2, dimensions.height / 2);
		clicState = 0;
	}
	void display(sf::RenderWindow * window);
	bool interact(sf::Vector2i mouse, bool clic);
	std::string getName();
};