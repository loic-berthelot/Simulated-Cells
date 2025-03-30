#pragma once
#include <SFML/Graphics.hpp>
#include "Cell.hpp"

class InfoPanel {
	sf::RenderTexture renderTexture;
	sf::RectangleShape sprite;
	sf::CircleShape cellPicture;
	sf::Vector2f size;
	sf::RectangleShape waterBar, oxygenBar, sugarBar;
	sf::RectangleShape maxWaterBar, maxOxygenBar, maxSugarBar;
	sf::RectangleShape cellFocus;
	sf::Text generalInformation, cellInformation, genesInformation;
	sf::Font font;
	Cell* myCell; // pointeur vers la cellule affichée
	Environment* Env = nullptr;

public:
	void updateCell(Cell* cell);
	InfoPanel(sf::Vector2f _size, Environment* Env) {
		this->Env = Env;
		size = _size;
		sprite.setPosition(sf::Vector2f(0, 0));
		sprite.setSize(sf::Vector2f(200, size.y));
		sprite.setFillColor(sf::Color(150, 150, 150));
		renderTexture.create(200, 1000);
		myCell = nullptr;

		maxWaterBar.setPosition(sf::Vector2f(25,  300));
		maxWaterBar.setFillColor(sf::Color(0, 0, 0));
		maxWaterBar.setSize(sf::Vector2f(150, 15));
		maxOxygenBar.setPosition(sf::Vector2f(25,  325));
		maxOxygenBar.setFillColor(sf::Color(0, 0, 0));
		maxOxygenBar.setSize(sf::Vector2f(150, 15));
		maxSugarBar.setPosition(sf::Vector2f(25, 350));
		maxSugarBar.setFillColor(sf::Color(0, 0, 0));
		maxSugarBar.setSize(sf::Vector2f(150, 15));

		waterBar.setPosition(sf::Vector2f(25,  300));
		waterBar.setFillColor(sf::Color(0, 50, 255));
		oxygenBar.setPosition(sf::Vector2f(25, 325));
		oxygenBar.setFillColor(sf::Color(200, 200, 200));
		sugarBar.setPosition(sf::Vector2f(25, 350));
		sugarBar.setFillColor(sf::Color(200, 50, 50));

		font.loadFromFile("../fonts/BalooPaaji2.ttf");

		generalInformation.setFont(font);
		generalInformation.setStyle(sf::Text::Bold);
		generalInformation.setCharacterSize(25);
		generalInformation.setFillColor(sf::Color::Black);
		generalInformation.setPosition(sf::Vector2f(15, 15));
		generalInformation.setLineSpacing(0.6);

		cellInformation.setFont(font);
		cellInformation.setStyle(sf::Text::Bold);
		cellInformation.setCharacterSize(16);
		cellInformation.setFillColor(sf::Color::Black);
		cellInformation.setPosition(sf::Vector2f(10, 380));
		cellInformation.setLineSpacing(0.6);

		genesInformation.setFont(font);
		genesInformation.setCharacterSize(15);
		genesInformation.setFillColor(sf::Color::Black);
		genesInformation.setPosition(sf::Vector2f(10, 500));
		genesInformation.setLineSpacing(0.6);
	}
	sf::RectangleShape getSprite();
	void display(sf::RenderWindow* window, int fps, bool fastMode, bool nightEnabled);
};