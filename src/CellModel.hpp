#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

class CellModel {
	std::vector<std::string> roles;
	std::vector<std::string> allRoles = {"photosynthesis", "fat", "root", "lung", "gill", "muscle", "bone", "reproducer", "water_reproducer", "poisonA", "poisonB", "poisonC", "antipoisonA", "antipoisonB", "antipoisonC", "explosive", "parasit", "antiparasit", "lifetime100", "lifetime300", "noLink", "link1", "link2", "link5", "electricIncreaser", "electricDecreaser", "glue", "quickClock" , "mediumClock" , "slowClock", "electricitySensitive", "pressureSensitive", "waterSensor", "radioactiveSensor", "neighborsSensor", "enemiesSensor", "noGift", "greatGift", "quickDuplication", "slowDuplication"};
	//, "childIndexNull", "childIndex_1", "childIndex_2", "childIndex_5", "childIndex_0_1", "childIndexRandom",
public :
	bool contains(std::string role);
	int count(std::string role);
	
	CellModel() {
		roles.clear();
	}
	CellModel(std::string text) {
		roles.clear();
		std::string word = "";
		for (int i = 0; i < text.length(); i++) {
			if (text[i] == ',') {
				roles.push_back(word);
				word = "";
			}
			else if (word != "" or text[i] != ' ') word += text[i];
		}
		if (word != "") roles.push_back(word);		
	}
	void mutate(CellModel* model, float mutationRate);
	CellModel(CellModel* model, float mutationRate) {
		mutate(model, mutationRate);
	}
	int getChildIndexShift();
	std::string getText();
};