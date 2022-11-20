#include "Environment.hpp"

void Environment::addCell(Cell* C) {
	this->cells.push_back(C);
}


void Environment::evolve() {
	for (int i = 0; i < cells.size(); i++) {
		cells[i]->clearNeighbors();
	}
	for (int i = 0; i < cells.size(); i++) {
		cells[i]->updateNeighbors();
		cells[i]->setElectricPower(0);
	}
	for (int i = 0; i < cells.size(); i++) {
		cells[i]->requestResources();
		cells[i]->updateElectricPower();
	}
	for (int i = 0; i < cells.size(); i++) {
		cells[i]->calculateResources();
		cells[i]->updateElectricCharge();
	}
	for (int i = 0; i < cells.size(); i++) {
		cells[i]->distributeResources();
		cells[i]->updateForces();
		cells[i]->move();
		cells[i]->checkHealth();
	}
}

void Environment::display(sf::RenderTexture& window, bool links, bool nightEnabled) {
	window.draw(sprite);
	sf::CircleShape* spr;
	Cell* cell2;
	std::vector<Cell*> neighbors;
	for (int c = 0; c < this->cells.size(); c++) {
		spr = cells[c]->getSprite();
		window.draw(*spr);
		if (links) {
			neighbors = cells[c]->getLinkedNeighbors();
			for (int i = 0; i < neighbors.size(); i++) {
				cell2 = neighbors[i];
				sf::Vertex line[] =
				{
					sf::Vertex(cells[c]->getPosition()),
					sf::Vertex(cell2->getPosition())
				};
				window.draw(line, 2, sf::Lines);
			}
		}		
	}
	window.draw(cellFocus->getSprite());
	if (nightEnabled and isNight()) window.draw(nightCache);
}

sf::IntRect Environment::getFrame() {
	return sf::IntRect(0, 0, (float) size.x, (float) size.y);
}

char Environment::getBiome(sf::Vector2f pos) {
	return biome_grid[(int) (pos.y / size.y * 100)][(int) (pos.x / size.x * 100)];
}

void Environment::updateCellFocus(sf::IntRect rect) {
	cellFocus->update(rect);
}

bool Environment::isNight() {
	return nightEnabled and frameNumber % (dayDuration+nightDuration) >= dayDuration;
}