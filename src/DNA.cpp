#include "DNA.hpp"


void DNA::parseText(std::string text) {
	models.clear();
	std::string word = "";
	CellModel* model;
	for (int i = 0; i < text.length(); i++) {
		if (text[i] == ';') {
			model = new CellModel(word);
			models.push_back(model);
			word = "";
		} else if (word != "" or text[i] != ' ') word += text[i];
	}
	if (word != "") {
		model = new CellModel(word);
		models.push_back(model);
	}
}

void DNA::mutate(DNA* dna, float mutationRate) {
	models.clear();
	int additiveMutations = rand()%3;
	for (int i = 0; i < dna->models.size(); i++) {
		if (rand() % 1001 >= mutationRate * 1000) models.push_back(new CellModel(dna->models[i], 0.05));
		for (int j = 0; j < additiveMutations; j++) {
			if (models.size() < 5 and rand() % 1001 < mutationRate * 1000) models.push_back(new CellModel(dna->models[i], 0.5));
		}
	}
	if (models.size() == 0) models.push_back(new CellModel(dna->models[rand()%dna->models.size()], 0.5));
}

void DNA::init() {
	color = sf::Color(rand() % 256, rand() % 256, rand() % 256);
	cellsNumber = 0;
	referenced = false;
}

sf::Color DNA::getColor() {
	return color;
}

int DNA::getGeneration() {
	return generation;
}

CellModel* DNA::getModel(int index) {
	return models[index];
}

int DNA::modelsNumber() {
	return models.size();
}

int DNA::getCellsNumber() {
	return cellsNumber;
}

void DNA::incrementCellsNumber() {
	cellsNumber++;
	if (not referenced) {
		referenced = true;
		static int refCompt = 0;
		refCompt++;
		refNumber = refCompt;
	}
}

void DNA::decrementCellsNumber() {
	cellsNumber--;
}

std::string DNA::getText() {
	std::string text = "";
	int size = models.size();
	for (int i = 0; i < size; i++) {
		text += models[i]->getText();
		if (i != size - 1) text += "; ";
	}
	return text;
}

float DNA::getRadius(int index) {
	float size = 20;
	CellModel* model = getModel(index);
	if (model->contains("lung")) size += 3;
	if (model->contains("gill")) size += 3;
	if (model->contains("explosive")) size += 3;
	if (model->contains("photosynthesis")) size += 5;
	if (model->contains("fat")) size += 10;
	return size;
}

int DNA::getRefNumber() {
	return refNumber;
}
