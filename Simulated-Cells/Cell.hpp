#pragma once
#include "commons.hpp"
#include "ResourceRequest.hpp"


class Environment;


class Cell
{
	sf::CircleShape sprite;
	sf::Vector2f speed, acceleration;
	float pressure, maxPressure, radius;
	float mass;
	float defaultSize; // La taille de la cellule lorsqu'elle ne subit aucune contrainte
	float roughness; // Plus la rugosité est importante, plus la cellule perd rapidement son élan
	sf::Color color, borderColor;
	std::vector<Cell*> neighbors; //Les cellules voisines, qui peuvent exercer une pression sur la cellule (et plus tard échanger des ressources et des signaux électriques)
	std::vector<Cell*> linkedNeighbors; //Les cellules voisines accrochées, qui peuvent exercer une traction sur la cellule
	Environment* Env;
	DNA* dna;
	int modelIndex;
	int maxLinks; // Le nombre maximal de voisines liées qu'une cellule peut avoir. cela vaut pour les cellules rencontrées ainsi que pour les nouvelles cellules.
	float elasticity; // L'élasticité est un coefficient qui, multiplié au rayon de la cellule, indique à partir de quelle distance la cellule est attirée par ses voisines liées
	bool dead;
	float reproductionRate; // Le premier argument est le numérateur et le second le dénominateur
	float randomMovementRate;
	sf::Vector2f movement;
	int timeAlive, lifetime, duplicationSpan, lastChildTime;
	float borderSize, sizeProportion;
	float water, oxygen, sugar;
	float waterMax, oxygenMax, sugarMax;
	float metabolism;
	float giftsMax;
	float electricCharge, electricPower, electricLoss;
	int nextIndex;
	std::vector<ResourceRequest*> resourcesRequests;

public:
	void removeInteraction(Cell* cell);
	bool cellFound(std::vector<Cell*>& cells, Cell* cell);
	bool hasRole(std::string role);
	int roleCount(std::string role);
	std::string getModelText();
	std::string getDnaText();
	std::string getFormatedCellText();
	std::string getFormatedDnaText(int lines, int columns, int columnsStrict);
	void updateRadius(float r);
	void applyRoles();
	char getBiome();
	float getRadius();
	float getWaterProportion();
	float getOxygenProportion();
	float getSugarProportion();
	Cell(sf::Vector2f position, Environment* Env, DNA* _dna = nullptr, int _modelIndex = 0) {
		sprite.setPosition(position);
		this->Env = Env;
		dna = _dna;
		if (dna == nullptr) dna = new DNA();
		dna->incrementCellsNumber();
		modelIndex = _modelIndex;
		
		pressure = 0;
		speed = sf::Vector2f(0, 0);
		acceleration = sf::Vector2f(0, 0);
		dead = false;
		color = dna->getColor();
		timeAlive = 0;
		borderSize = 2;

		neighbors.clear();
		linkedNeighbors.clear();
		sprite.setFillColor(color);

		applyRoles();
		
		if (borderSize != 0) {
			sprite.setOutlineThickness(borderSize);
			sprite.setOutlineColor(borderColor);
		}
	}
	
	~Cell() {
		for (int i = 0; i < neighbors.size(); i++) {
			neighbors[i]->removeInteraction(this);
		}
		for (int i = 0; i < linkedNeighbors.size(); i++) {
			linkedNeighbors[i]->removeInteraction(this);
		}
		dna->decrementCellsNumber();
		if (dna->getCellsNumber() == 0) delete dna;
	}
	sf::CircleShape* getSprite(); //Au cas où on fait le travail proprement avec des getters
	void updateNeighbors();
	void addOxygen(float amount);
	void addWater(float amount);
	void addSugar(float amount);
	void consumeOxygen(float amount);
	void consumeWater(float amount);
	void consumeSugar(float amount);
	void updateForces();
	void move();
	void accelerate(sf::Vector2f force);
	void die();
	void checkHealth();
	sf::Vector2f getPosition();
	int nextModelIndex();
	void duplicate();
	bool addToNeighbors(Cell* cell);
	bool addToLinkedNeighbors(Cell* cell);
	bool removeFromNeighbors(Cell* cell);
	bool removeFromLinkedNeighbors(Cell* cell);
	void clearNeighbors();
	bool isDead();
	sf::Vector2f getVelocity();
	float getMass();
	void addResourceRequest(ResourceRequest * request);
	float calculateRequestAmount(float val1, float max1, float val2, float max2);
	void requestResources();
	void calculateResources();
	void distributeResources();	
	float getElectricCharge();
	void addElectricPower(float power);
	void setElectricPower(float power);
	void updateElectricPower();
	void updateElectricCharge();
	float getElectricPower();
	sf::Color getColor();
	std::vector<Cell*> getLinkedNeighbors();
};

