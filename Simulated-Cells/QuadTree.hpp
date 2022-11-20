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

	// Constante arbitraire indiquant combien d'éléments peuvent être stockés dans ce nœud de quadtree
	const int QT_NODE_CAPACITY = 5;


	// Zone de délimitation alignée sur l'axe (représentée par sa demi-dimension et son centre)
	// représentant les limites de ce quadtree
	RectByCenter boundary;

	// Points de ce nœeud de quadtree
	std::vector <Cell*> points;


	// Méthodes
	QuadTree(RectByCenter bd) { this->boundary = bd; };

	bool insert(Cell* p);
	void subdivide(); // créer quatre enfants permettant de diviser ce quadrant en quatre quadrants d'égales dimensions
	void queryRangeRect(RectByCenter range, std::vector<Cell*> & pointsInRange);
	void queryRangeCircle(RectByCenter range, std::vector<Cell*>& pointsInRange);
	void display(sf::RenderTexture& window);
	void del();  // delete the QuadTree from the actual pos. to the leafs

};

