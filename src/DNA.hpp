#pragma once
#include "CellModel.hpp"

class DNA {
	sf::Color color;
	std::vector<CellModel*> models;
	int cellsNumber, refNumber, generation;
	bool referenced;
public:
	void parseText(std::string text);
	void mutate(DNA* dna, float mutationRate);
	void init();
	DNA() {
		init();
		models.clear();
		CellModel* model = new CellModel();
		models.push_back(model);
		generation = 1;
	}
	DNA(DNA* dna, float mutationRate=0) {
		init();
		mutate(dna, mutationRate);
		generation = dna->getGeneration() + 1;
	}
	DNA(std::string text) {
		init();
		parseText(text);
		generation = 1;
	}
	~DNA() {
		for (int i = 0; i < models.size(); i++) {
			delete models[i];
		}
	}
	sf::Color getColor();
	int getGeneration();
	CellModel* getModel(int index);
	int modelsNumber();
	int getCellsNumber();
	void incrementCellsNumber();
	void decrementCellsNumber();
	std::string getText();
	float getRadius(int index);
	int getRefNumber();
};