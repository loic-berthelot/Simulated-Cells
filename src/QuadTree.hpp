#pragma once
#include "commons.hpp"



class Cell;

class QuadTree
{
public:
	// Enfants/*
	QuadTree* northWest;
	QuadTree* northEast;
	QuadTree* southWest;
	QuadTree* southEast;

	// Constante arbitraire indiquant combien d'�l�ments peuvent �tre stock�s dans ce n�ud de quadtree
	const int QT_NODE_CAPACITY = 5;


	// Zone de d�limitation align�e sur l'axe (repr�sent�e par sa demi-dimension et son centre)
	// repr�sentant les limites de ce quadtree
	RectByCenter boundary;

	// Points de ce n�eud de quadtree
	std::vector <Cell*> points;


	// M�thodes
	QuadTree(RectByCenter bd) {
		this->boundary = bd;
		northWest = nullptr;
		northEast = nullptr;
		southWest = nullptr;
		southEast = nullptr;
	};

	bool insert(Cell* p);
	void subdivide(); // cr�er quatre enfants permettant de diviser ce quadrant en quatre quadrants d'�gales dimensions
	void queryRangeRect(RectByCenter range, std::vector<Cell*> & pointsInRange);
	void queryRangeCircle(RectByCenter range, std::vector<Cell*>& pointsInRange);
	void display(sf::RenderTexture& window);
	void del();  // delete the QuadTree from the actual pos. to the leafs

};

