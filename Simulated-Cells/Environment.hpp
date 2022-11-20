#pragma once
#include "commons.hpp"

extern bool nightEnabled;

class QuadTree;

class Environment
{
public:
	sf::Texture _spriteTexture;
	sf::Sprite sprite;
	std::vector<Cell*> cells;
	sf::Vector2u size;
	sf::RectangleShape nightCache;

	QuadTree* Quad;

	CellFocus* cellFocus;

	sf::Image background;
	char biome_grid[100][100]; //number of cells/chunks in the total grid
	int frameNumber;
	int dayDuration, nightDuration;

	Environment(QuadTree* Quad, sf::Vector2f mapSize, std::string mapName) {
		sprite.setPosition(sf::Vector2f(0, 0));
		sprite.setOrigin(sf::Vector2f(0, 0));
		size = sf::Vector2u(mapSize.x, mapSize.y);
		cells.clear();

		RectByCenter bd(sf::Vector2f(size.x / 2, size.y / 2), sf::Vector2f(size.x / 2, size.y / 2));
		this->Quad = Quad;
		cellFocus = new CellFocus();

		sf::Image originalBackground; originalBackground.loadFromFile("../img/backgrounds/"+mapName+".png");
		background.create(size.x, size.y);
		resizeImage(std::ref(originalBackground), std::ref(background));
		// (128, 64, 0) = dirt,  (63, 72, 204) = water  ,  (34, 177, 76) = radioactive
		float xDiff = std::ceil(size.x / 100); float yDiff = std::ceil(size.y / 100);
		for (int x = 0; x < 100; x++) {
			for (int y = 0; y < 100; y++) {
				sf::Color c = background.getPixel(std::min((int) (x * xDiff), (int)size.x), std::min((int) (y * yDiff), (int)size.y));
				if (c == sf::Color(128, 64, 0)) {
					biome_grid[y][x] = 'd';
				} else if (c == sf::Color(63, 72, 204)) {
					biome_grid[y][x] = 'w';
				} else if (c == sf::Color(34, 177, 76)) {
					biome_grid[y][x] = 'r';
				} else if (c == sf::Color(240, 30, 100)) {
					biome_grid[y][x] = 's';
				}
				else {
					biome_grid[y][x] = 'd';
				}
			}
		}

		
		_spriteTexture.loadFromImage(background);
		sprite.setTexture(std::ref(_spriteTexture));
		frameNumber = 0;
		dayDuration = 2000;
		nightDuration = 2000;

		nightCache.setSize(mapSize);
		nightCache.setFillColor(sf::Color(0, 0, 0, 75));
	}

	void addCell(Cell* C);
	void evolve();
	void display(sf::RenderTexture& window, bool links, bool nightEnabled);
	sf::IntRect getFrame();
	char getBiome(sf::Vector2f pos);
	void updateCellFocus(sf::IntRect rect);
	bool isNight();
};