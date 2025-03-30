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
	float roughness; // Plus la rugosit� est importante, plus la cellule perd rapidement son �lan
	sf::Color color, borderColor;
	std::vector<Cell*> neighbors; //Les cellules voisines, qui peuvent exercer une pression sur la cellule (et plus tard �changer des ressources et des signaux �lectriques)
	std::vector<Cell*> linkedNeighbors; //Les cellules voisines accroch�es, qui peuvent exercer une traction sur la cellule
	Environment* Env;
	DNA* dna;
	int modelIndex;
	int maxLinks; // Le nombre maximal de voisines li�es qu'une cellule peut avoir. cela vaut pour les cellules rencontr�es ainsi que pour les nouvelles cellules.
	float elasticity; // L'�lasticit� est un coefficient qui, multipli� au rayon de la cellule, indique � partir de quelle distance la cellule est attir�e par ses voisines li�es
	bool dead;
	float reproductionRate; // Le premier argument est le num�rateur et le second le d�nominateur
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
	float calculateMetabolism();
	int calculateDuplicationSpan();
	float getWaterProportion();
	float getOxygenProportion();
	float getSugarProportion();
	Cell(sf::Vector2f position, Environment* Env, DNA* _dna = nullptr, int _modelIndex = 0);
	~Cell();
	sf::CircleShape* getSprite(); //Au cas o� on fait le travail proprement avec des getters
	void updateNeighbors();
	void addOxygen(float amount);
	void addWater(float amount);
	void addSugar(float amount);
	void consumeOxygen(float amount);
	void consumeWater(float amount);
	void consumeSugar(float amount);
	void updateForces();
	void checkLifetime();
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

