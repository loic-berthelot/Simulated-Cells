#include "CellModel.hpp"

bool CellModel::contains(std::string role) {
	for (int i = 0; i < roles.size(); i++) {
		if (roles[i] == role) return true;
	}
	return false;
}

int CellModel::count(std::string role) {
	int n = 0;
	for (int i = 0; i < roles.size(); i++) {
		if (roles[i] == role) n++;
	}
	return n;
}

int CellModel::getChildIndexShift() { 
	int childIndexShift;
	if (contains("childIndexRandom")) return rand();
	if (contains("childIndexNull")) childIndexShift = 0;
	else childIndexShift = 1;
	if (contains("childIndex_1")) childIndexShift += count("childIndex_1");
	if (contains("childIndex_2")) childIndexShift += 2*count("childIndex_2");
	if (contains("childIndex_5")) childIndexShift += 5*count("childIndex_5");	
	if (contains("childIndex_0_1")) childIndexShift += rand() % (1+count("childIndex_0_1"));
	return childIndexShift; 
}

std::string CellModel::getText() {
	std::string text = "";
	int size = roles.size();
	for (int i = 0; i < size; i++) {
		text += roles[i];
		if (i != size -1) text += ", ";
	}
	return text;
}

void CellModel::mutate(CellModel* model, float mutationRate) {
	roles.clear();
	for (int i = 0; i < model->roles.size(); i++) {
		if (rand() % 1001 >= mutationRate * 1000) roles.push_back(model->roles[i]);
		for (int j = 0; j < 3; j++) {
			if (roles.size() < 5 and rand() % 1001 < mutationRate * 1000) roles.push_back(allRoles[rand() % allRoles.size()]);
		}
	}
	if (roles.size() == 0) roles.push_back(allRoles[rand() % allRoles.size()]);
}