#include "InfoPanel.hpp"

sf::RectangleShape InfoPanel::getSprite() {
	return sprite;
}

void InfoPanel::updateCell(Cell* cell) {
	myCell = cell;
	if (cell != nullptr) {
		cellPicture = *cell->getSprite();
		cellPicture.setPosition(sf::Vector2f(100, 200));
		float radius = cellPicture.getRadius();
		cellPicture.setOrigin(sf::Vector2f(radius, radius));
	}
}

void InfoPanel::display(sf::RenderWindow* window, int fps, bool fastMode, bool nightEnabled) {
	window->draw(sprite);
	std::string text;
	if (Env != nullptr) {
		text = "Frame : " + std::to_string(Env->frameNumber) + "\nFPS ";
		if (fastMode) text += "[fast] ";
		text += ": "+std::to_string(fps);
		if (nightEnabled) {
			if (Env->isNight()) text += "\nNight";
			else text += "\nDay";
		}
		generalInformation.setString(text);
		window->draw(generalInformation);
	}
	if (myCell != nullptr) {
		window->draw(maxWaterBar);
		window->draw(maxOxygenBar);
		window->draw(maxSugarBar);
		
		waterBar.setSize(sf::Vector2f(150 * myCell->getWaterProportion(), 15));
		oxygenBar.setSize(sf::Vector2f(150 * myCell->getOxygenProportion(), 15));
		sugarBar.setSize(sf::Vector2f(150 * myCell->getSugarProportion(), 15));
		window->draw(waterBar);
		window->draw(oxygenBar);
		window->draw(sugarBar);
			
		window->draw(cellPicture);
		
		text = myCell->getFormatedCellText();
		cellInformation.setString(text);
		window->draw(cellInformation);

		text = myCell->getFormatedDnaText(25, 20, 28);
		genesInformation.setString(text);
		window->draw(genesInformation);
	}
}