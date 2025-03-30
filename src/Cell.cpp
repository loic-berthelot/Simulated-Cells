#include "Cell.hpp"


Cell::Cell(sf::Vector2f position, Environment* Env, DNA* _dna, int _modelIndex) {
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

Cell::~Cell() {
	for (int i = 0; i < neighbors.size(); i++) {
		neighbors[i]->removeInteraction(this);
	}
	for (int i = 0; i < linkedNeighbors.size(); i++) {
		linkedNeighbors[i]->removeInteraction(this);
	}
	dna->decrementCellsNumber();
	if (dna->getCellsNumber() == 0) delete dna;
}

void Cell::removeInteraction(Cell* cell)
{
	for (int i = 0; i < neighbors.size(); i++) {
		if (neighbors[i] == cell) neighbors.erase(neighbors.cbegin() + i);
	}
	for (int i = 0; i < linkedNeighbors.size(); i++) {
		if (linkedNeighbors[i] == cell) linkedNeighbors.erase(linkedNeighbors.cbegin() + i);
	}
}

sf::CircleShape* Cell::getSprite() {
	return &this->sprite;
}

sf::Vector2f Cell::getPosition() {
	return sprite.getPosition();
}

void Cell::updateRadius(float r) {
	radius = r;
	if (borderSize < radius) sprite.setRadius(radius-borderSize);
	else sprite.setRadius(borderSize);
	sprite.setOrigin(radius, radius);
}

char Cell::getBiome() { 
	return Env->getBiome(getPosition());  
}

float Cell::getRadius() {
	return radius;
}
float Cell::calculateMetabolism() {
	float result = mass * 0.0001;
	if (hasRole("lung")) result += 0.03;
	if (hasRole("gill")) result += 0.03;
	if (hasRole("root")) result += 0.05;
	if (hasRole("fat")) result += 0.03;
	if (hasRole("photosynthesis")) result += 0.02;
	if (hasRole("poisonA")) result += 1;
	if (hasRole("poisonB")) result += 0.7;
	if (hasRole("poisonC")) result += 0.4;
	if (hasRole("antipoisonA")) result += 0.2;
	if (hasRole("antipoisonB")) result += 0.14;
	if (hasRole("antipoisonC")) result += 0.08;
	if (hasRole("explosive")) result += 0.01;
	if (hasRole("parasit")) result += 0.03;
	if (hasRole("antiparasit")) result += 0.06;
	if (hasRole("bone")) result += 0.07;
	/*
	if (hasRole("electricIncreaser")+hasRole("electricDecreaser")==1) result += 0.01;
	if (hasRole("quickClock") or hasRole("mediumClock") or hasRole("slowClock")) result += 0.01;
	if (hasRole("waterSensor") or hasRole("radioactiveSensor") or hasRole("neighborsSensor") or hasRole("enemiesSensor")) result += 0.1;*/
	result *= 1;
	return result;
}

int Cell::calculateDuplicationSpan() {
	int result;
	if (hasRole("quickDuplication")) result = 30;
	else if (hasRole("slowDuplication")) result = 120;
	else result = 60;
	return result;
}
void Cell::applyRoles() {
	borderColor = sf::Color::White;
	defaultSize = dna->getRadius(modelIndex);
	reproductionRate = 0.005;
	maxPressure = defaultSize * 0.5;
	randomMovementRate = 1;
	roughness = 0.05;
	elasticity = 0.2;
	if (hasRole("bone"))  {
		maxPressure *= 2;
		elasticity = 0.05;
	} else if (hasRole("fat")) {
		maxPressure *= 2;
	}
	mass = defaultSize * defaultSize;
	metabolism = calculateMetabolism();
	if (hasRole("lifetime300")) {
		lifetime = 300;
	} else if (hasRole("lifetime100")) {
		lifetime = 100;
	} else {
		lifetime = 500;// -1;
	}
	if (hasRole("pressureSensitive")) maxPressure *= 0.33333;
	else metabolism += 0.02;
	if (not hasRole("electricitySensitive")) metabolism += 0.02;

	if (hasRole("noLink")) maxLinks = 0;
	else maxLinks = 1000;
	maxLinks += roleCount("link1") + 2 * roleCount("link2") + 5 * roleCount("link5");
	duplicationSpan = calculateDuplicationSpan();
	lastChildTime = Env->frameNumber;

	updateRadius(defaultSize);
	waterMax = 0.33 * mass;
	oxygenMax = 0.33 * mass;
	sugarMax = 0.33 * mass;
	if (hasRole("fat")) {
		waterMax *= 2;
		oxygenMax *= 2;
		sugarMax *= 2;
	}
	if (Env->frameNumber == 0) {
		water = waterMax;
		oxygen = oxygenMax;
		sugar = sugarMax;
	}
	else {
		water = 0.25*waterMax;
		oxygen = 0.25*oxygenMax;
		sugar = 0.25*sugarMax;
	}
	if (hasRole("noGift")) giftsMax = 0;
	else if (hasRole("greatGift")) giftsMax = 0.01;
	else giftsMax = 0.002;
	sizeProportion = 1;
	electricCharge = 0;
	electricPower = 0;
	electricLoss = 0.05;
	nextIndex = -1;
	if (hasRole("explosive")) borderColor = sf::Color::Black;
	else if (hasRole("poisonA") or hasRole("poisonB") or hasRole("poisonC")) borderColor = sf::Color::Red;
	else if (hasRole("photosynthesis")) borderColor = sf::Color::Green;
	else if (hasRole("lung")) borderColor = sf::Color::Blue;
	else if (hasRole("gill")) borderColor = sf::Color::Yellow;
	else if (hasRole("reproducer") or hasRole("water_reproducer")) borderColor = sf::Color::Magenta;
	else if (hasRole("glue")) borderColor = sf::Color::Cyan;
}

bool Cell::cellFound(std::vector<Cell*> & cells, Cell* cell) {
	for (int i = 0; i < cells.size(); i++) {
		if (cells[i] == cell) return true;
	}
	return false;
}

bool Cell::hasRole(std::string role) {
	return dna->getModel(modelIndex)->contains(role);
}

int Cell::roleCount(std::string role) {
	return dna->getModel(modelIndex)->count(role);
}

std::string Cell::getModelText() {
	return dna->getModel(modelIndex)->getText();
}

std::string Cell::getDnaText() {
	return dna->getText();
}

std::string Cell::getFormatedCellText() {
	std::string text = "Electric charge : " + std::to_string((int)electricCharge) + "\nElectric power : " + std::to_string((int)electricPower) + "\nMetabolism : " + std::to_string(metabolism) + "\nPressure : " + std::to_string(pressure);
	return text;
}

std::string Cell::getFormatedDnaText(int lines, int columns, int columnsStrict) {
	std::string text = "Ref : " + std::to_string(dna->getRefNumber()) + "\nCells number : " + std::to_string(dna->getCellsNumber()) + "\nGeneration : " + std::to_string(dna->getGeneration()) + "\n\nGenes : \n";
	std::string line, modelText;
	int j;
	int lineCompt = 1;
	for (int i = 0; i < dna->modelsNumber(); i++) {
		if (i == modelIndex) line = "   >> ";
		else line = "- ";
		modelText = dna->getModel(i)->getText();
		for (int j = 0; j < modelText.length(); j++) {
			if (modelText[j] != ' ' or line.length() > 0) line += modelText[j];
			if ((line.length() >= columns and modelText[j] == ',') or line.length() >= columnsStrict) {
				text += line + "\n";
				lineCompt++;
				if (lineCompt >= lines) return text;
				line = "";
			}
		}
		text += line + "\n";
		lineCompt++;
		if (lineCompt >= lines) return text;
	}
	return text;
}

void Cell::clearNeighbors() {
	this->neighbors.clear();
}

bool Cell::addToNeighbors(Cell* cell) {
	if (cellFound(neighbors, cell)) return false;
	neighbors.push_back(cell);
	cell->neighbors.push_back(this);
	return true;
}

bool Cell::addToLinkedNeighbors(Cell* cell) {
	if (cellFound(linkedNeighbors, cell)) return false;
	if (linkedNeighbors.size() >= maxLinks) return false;
	if (cell->linkedNeighbors.size() >= cell->maxLinks) return false;
	linkedNeighbors.push_back(cell);
	cell->linkedNeighbors.push_back(this);
	return true;
}

bool Cell::removeFromNeighbors(Cell* cell) {
	for (int i = 0; i < neighbors.size(); i++) {
		if (neighbors[i] == cell) {
			for (int j = 0; j < cell->neighbors.size(); j++) {
				if (cell->neighbors[j] == this) {
					cell->neighbors.erase(cell->neighbors.begin() + j);
					continue;
				}
			}
			neighbors.erase(neighbors.begin() + i);
			return true;
		}
	}
	return false;
}

bool Cell::removeFromLinkedNeighbors(Cell* cell) {
	for (int i = 0; i < linkedNeighbors.size(); i++) {
		if (linkedNeighbors[i] == cell) {
			for (int j = 0; j < cell->linkedNeighbors.size(); j++) {
				if (cell->linkedNeighbors[j] == this) {
					cell->linkedNeighbors.erase(cell->linkedNeighbors.begin() + j);
					continue;
				}
			}
			linkedNeighbors.erase(linkedNeighbors.begin() + i);
			return true;
		}
	}
	return false;
};

void Cell::updateNeighbors() {
	std::vector<Cell*> cells;
	RectByCenter rect;
	rect.center = getPosition();
	rect.radius = sf::Vector2f(2*radius, 2*radius);
	this->Env->Quad->queryRangeCircle(rect, cells);
	for (int i = 0; i < cells.size(); i++) {
		if (this != cells[i]) {
			addToNeighbors(cells[i]);
			if ((color == cells[i]->color or hasRole("parasit")) and distanceBetweenPoints(this->getPosition(), cells[i]->getPosition()) <= this->radius + cells[i]->radius) addToLinkedNeighbors(cells[i]);
		}
	}
}


void Cell::addOxygen(float amount) {
	oxygen += amount;
	if (oxygen > oxygenMax) oxygen = oxygenMax;
}

void Cell::addWater(float amount) {
	water += amount;
	if (water > waterMax) water = waterMax;
}

void Cell::addSugar(float amount) {
	sugar += amount;
	if (sugar > sugarMax) sugar = sugarMax;
}

void Cell::consumeOxygen(float amount) {
	oxygen -= amount;
	if (oxygen < 0) oxygen = 0;
	if(oxygen == 0) die();
}

void Cell::consumeWater(float amount) {
	water -= amount;
	if (water < 0) water = 0;
	if (water == 0) die();
}

void Cell::consumeSugar(float amount) {
	sugar -= amount;
	if (sugar < 0) sugar = 0;
	if (sugar == 0) die();
}

float Cell::getWaterProportion() {
	if (waterMax == 0) return 0;
	return water / waterMax;
}
float Cell::getOxygenProportion() {
	if (oxygenMax == 0) return 0;
	return oxygen / oxygenMax;
}
float Cell::getSugarProportion() {
	if (sugarMax == 0) return 0;
	return sugar / sugarMax;
}

void Cell::updateForces() { //On considère pour l'instant que toutes les cellules exercent la même force, indépendamment de leur taille
	if (hasRole("muscle")) {
		float constriction;
		float constrictionMax = 0.4;
		constriction = 0.05-0.005*std::min(electricPower, 50.f);
		sizeProportion += constriction;
		if (sizeProportion > 1) sizeProportion = 1;
		else if (sizeProportion < 1 - constrictionMax) sizeProportion = 1 - constrictionMax;
		/*int periodDuration = 120;
		int periodTime = timeAlive % (periodDuration + 1);
		if (periodTime >= periodDuration /2) coef = 1 - constriction + constriction * periodTime / periodDuration;
		else coef = 1 - constriction + constriction * (periodDuration - periodTime - 1) / periodDuration;
		*/
		updateRadius(defaultSize * sizeProportion);
	}
	if (hasRole("glue")) {
		if (electricPower > 10) {
			roughness = 0.05;
			borderSize = 3;
			sprite.setOutlineThickness(borderSize);
			updateRadius(radius);
		} else {
			roughness = 0.95;
			borderSize = 6;
			sprite.setOutlineThickness(borderSize);
			updateRadius(radius);
		}
		//roughness = 0.05 + 0.4 * std::max(electricPower, 0.f);
		//if (roughness > 0.95) roughness = 0.95;
	}
							
	acceleration = sf::Vector2f(0, 0);
	pressure = 0;
	//acceleration = multiplyVector(sf::Vector2f(rand() % 31 - 15, rand() % 31 - 15), 0.03);
	float product, dist, radiusSum;
	sf::Vector2f neighborDirection, mtd;
	for (int i = 0; i < neighbors.size(); i++) {
		Cell* cell1 = this;
		Cell* cell2 = neighbors[i];

		neighborDirection = substractVectors(cell2->getPosition(), cell1->getPosition());
		dist = vectorNorm(neighborDirection);
		radiusSum = cell1->radius + cell2->radius;
		if (dist >= radiusSum) continue;

		float m1 = 1 / cell2->getMass();
		float m2 = 1 / cell2->getMass();
		mtd = normalizeVector(neighborDirection, (dist - radiusSum) * m2 / (m1 + m2));
		movement = addVectors(movement, mtd);
		pressure += vectorNorm(mtd, 0.001);

		if (color != cell2->getColor()) {
			if ( (hasRole("poisonA") and not cell2->hasRole("antipoisonA")) or (hasRole("poisonB") and not cell2->hasRole("antipoisonB")) or (hasRole("poisonC") and not cell2->hasRole("antipoisonC")) ) {
				if ((cell2->hasRole("poisonA") and not hasRole("antipoisonA")) or (cell2->hasRole("poisonB") and not hasRole("antipoisonB")) or (cell2->hasRole("poisonC") and not hasRole("antipoisonC"))) die();
				cell2->die();
			}
		}
		if (hasRole("electricitySensitive") and abs(electricPower) > 15) die();
	}

	int i = 0;
	float size = linkedNeighbors.size();
	while (i < size) {
		Cell* cell2 = linkedNeighbors[i];
		float mass2 = cell2->getMass();
		neighborDirection = substractVectors(cell2->getPosition(), getPosition());
		float dist = vectorNorm(neighborDirection); // distance entre ma cellule et la cellule courante
		if (dist > (1+elasticity) * radius + (1+cell2->elasticity) * cell2->radius) {
			removeFromLinkedNeighbors(cell2);
		} else {
			if (dist > (1+0.2*elasticity) * radius + (1 + 0.2 *cell2->elasticity) * cell2->radius) {
				float forceFactor = 0.5*mass2/(mass+mass2)*((1+elasticity) * radius + (1+cell2->elasticity) * cell2->radius - dist);
				movement = addVectors(movement, normalizeVector(neighborDirection, forceFactor));
			}
			i++;
		}
		size = linkedNeighbors.size();
	}
	
}

void Cell::checkLifetime() {
	timeAlive++;
	if (lifetime > 0) lifetime--;
	else die();
}

void Cell::move() {
	sprite.move(multiplyVector(movement, 1-roughness));
	movement = sf::Vector2f(0, 0);
	speed = multiplyVector(addVectors(speed, movement), 1 - roughness);
	sprite.move(speed);

	//if out of screen
	
	sf::Vector2f posT = getPosition();
	sf::IntRect frame = Env->getFrame();
	if (not pointInFrame(posT, frame, radius)) {
		sprite.setPosition(sf::Vector2f(std::fmax(radius, std::fmin(posT.x, frame.width - radius)), std::fmax(radius, std::fmin(posT.y, frame.height - radius))));
		speed = sf::Vector2f(0, 0);
	}

	if (hasRole("root")) addWater(0.01*abs(electricPower)*radius);
	if (getBiome() == 'w' || getBiome() == 's'|| getBiome() == 'r') {
		addWater(0.04 * radius);
		if (hasRole("gill")) addOxygen(0.2);
	} else {
		if (hasRole("lung")) addOxygen(0.2);
	}
	if (getBiome() == 's' || getBiome() == 'r' ) {
		addSugar(0.2 * radius);
	}
	float photosynthesisSpeed = 0;
	if (hasRole("photosynthesis") && (getBiome() =='l' || !Env->isNight())) {
		photosynthesisSpeed = 1;
		if (getBiome() =='l' && !Env->isNight()) photosynthesisSpeed = 2;
	}
	addOxygen(photosynthesisSpeed * water / waterMax);
	addSugar(photosynthesisSpeed * water / waterMax);
	consumeWater(photosynthesisSpeed * water / waterMax);
	consumeOxygen(metabolism);
	consumeSugar(metabolism);
}

void Cell::updateElectricPower() {
	float power;
	float size = linkedNeighbors.size();
	for (int i = 0; i < size; i++) {
		power = 0.015*(electricCharge - linkedNeighbors[i]->getElectricCharge())/(size+1);
		electricPower -= power;
		linkedNeighbors[i]->addElectricPower(power);	
	}
}

void Cell::updateElectricCharge() {
	electricCharge += electricPower;
	if (hasRole("electricIncreaser") and not hasRole("electricDecreaser")) electricCharge += 100;
	if (hasRole("electricDecreaser") and not hasRole("electricIncreaser")) electricCharge -= 100;
	if (hasRole("quickClock")) electricCharge += 200 * sin(timeAlive / 20);
	if (hasRole("mediumClock")) electricCharge += 200 * sin(timeAlive / 50);
	if (hasRole("slowClock")) electricCharge += 200 * sin(timeAlive / 90);
	if (hasRole("waterSensor") and getBiome() == 'w') electricCharge += 100;
	if (hasRole("radioactiveSensor") and getBiome() == 'r') electricCharge += 100;
	if (hasRole("neighborsSensor")) electricCharge += 20 * neighbors.size();
	if (hasRole("enemiesSensor")) {
		for (int i = 0; i < neighbors.size(); i++) {
			if (neighbors[i]->getColor() != color) electricCharge += 20;
		}
	}
	electricCharge -= electricLoss * electricCharge;
}

void Cell::accelerate(sf::Vector2f force) {
	movement = addVectors(movement, multiplyVector(force, 1/mass));
}

void Cell::die() {
	if (not dead) {
		float waterAmount = 0.66 * water + 0.066 * mass;
		float oxygenAmount = 0.66 * oxygen + 0.066 * mass;
		float sugarAmount = 0.66 * sugar + 0.066 * mass;
		float size = neighbors.size();
		for (int i = 0; i < size; i++) {
			neighbors[i]->addWater(waterAmount / size);
			neighbors[i]->addOxygen(oxygenAmount / size);
			neighbors[i]->addSugar(sugarAmount / size);
		}
		if (hasRole("explosive")) {
			std::vector<Cell*> cells;
			RectByCenter rect;
			rect.center = getPosition();
			rect.radius = sf::Vector2f(100, 100);
			this->Env->Quad->queryRangeCircle(rect, cells);
			for (int i = 0; i < cells.size(); i++) {
				if (this != cells[i]) {
					sf::Vector2f neighborDirection = substractVectors(cells[i]->getPosition(), getPosition());
					float dist = vectorNorm(neighborDirection)-cells[i]->radius;
					if (dist <= 100) cells[i]->accelerate(normalizeVector(neighborDirection, 1000000) / dist);
				}
			}
		}
		dead = true;
	}
}

void Cell::checkHealth() {
	if (pressure > maxPressure) die();
}

int Cell::nextModelIndex() {
	if (nextIndex == -1) nextIndex = (modelIndex + 1+0*dna->getModel(modelIndex)->getChildIndexShift()) % dna->modelsNumber();
	return nextIndex; ;
}

void Cell::duplicate() {
	if (linkedNeighbors.size() < maxLinks && Env->frameNumber - lastChildTime >= duplicationSpan && pressure <= 1 && rand()%10 == 0) {
		float mutationRate = 0;
		if (rand() % 100 == 0) mutationRate += 0.15;
		if (hasRole("reproducer") && rand() % 4 == 0) mutationRate += 0.15;
		else if (hasRole("water_reproducer") && getBiome() == 'w' && rand() % 4 == 0) mutationRate += 0.15;
		if (getBiome() == 'r' && rand() % 4 == 0) mutationRate += 0.15;
		DNA* dna2;
		int nextIndex;
		if (mutationRate > 0) {
			dna2 = new DNA(dna, mutationRate);
			nextIndex = 0;
		} else {
			dna2 = dna;
			nextIndex = nextModelIndex();
		}
		float size2 = dna2->getRadius(nextIndex);
		float mass2 = size2 * size2;
		if (water >= 5 + 0.1 * mass2 and oxygen >= 5 + 0.1 * mass2 and sugar >= 5 + 0.1 * mass2) {
			consumeWater(0 + 0.1 * mass2);
			consumeOxygen(0 + 0.1 * mass2);
			consumeSugar(0 + 0.1 * mass2);
			Cell* cell = new Cell(getPosition(), Env, dna2, nextIndex);
			Env->addCell(cell);
			cell->sprite.setPosition(addVectors(sprite.getPosition(), normalizeVector(acceleration, radius+cell->radius)));
			addToNeighbors(cell);
			addToLinkedNeighbors(cell);
			nextIndex = -1;
			lastChildTime = Env->frameNumber;
		}
		else if (dna2 != dna) delete dna2;
	}
	
}

bool Cell::isDead() { return dead; }

sf::Vector2f Cell::getVelocity() { return speed; }

float Cell::getMass() { return mass; }

void Cell::addResourceRequest(ResourceRequest * request) {
	resourcesRequests.push_back(request);
}

float Cell::calculateRequestAmount(float val1, float max1, float val2, float max2) {
	float percentage1 = val1 / max1;
	float percentage2 = val2 / max2;
	if (percentage1 >= percentage2) return 0;
	return (percentage2 - percentage1) * max1;
}

void Cell::requestResources() {
	for (int i = 0; i < linkedNeighbors.size(); i++) {
		Cell* cell2 = linkedNeighbors[i];
		float waterNeeded = calculateRequestAmount(water, waterMax, cell2->water, cell2->waterMax);
		float oxygenNeeded = calculateRequestAmount(oxygen, oxygenMax, cell2->oxygen, cell2->oxygenMax);
		float sugarNeeded = calculateRequestAmount(sugar, sugarMax, cell2->sugar, cell2->sugarMax);
		if (waterNeeded > 0 or oxygenNeeded > 0 or sugarNeeded > 0) cell2->addResourceRequest(new ResourceRequest(this, waterNeeded, oxygenNeeded, sugarNeeded));
	}
}

void Cell::calculateResources() {
	float waterNeeded = 0;
	float oxygenNeeded = 0;
	float sugarNeeded = 0;
	float waterRatio;
	float oxygenRatio;
	float sugarRatio;
	float waterAvailable = water;
	float oxygenAvailable = oxygen;
	float sugarAvailable = sugar;
	for (int i = 0; i < resourcesRequests.size(); i++) {
		if (color == resourcesRequests[i]->cell->getColor() or not hasRole("antiparasit")) {
			waterNeeded += resourcesRequests[i]->water;
			oxygenNeeded += resourcesRequests[i]->oxygen;
			sugarNeeded += resourcesRequests[i]->sugar;
		}
	}
	
	if (giftsMax * waterMax < waterAvailable) waterAvailable = giftsMax * waterMax;
	if (giftsMax * oxygenMax < oxygenAvailable) oxygenAvailable = giftsMax * oxygenMax;
	if (giftsMax * sugarMax < sugarAvailable) sugarAvailable = giftsMax * sugarMax;
	if (waterAvailable >= waterNeeded) waterRatio = 1;
	else waterRatio = waterAvailable / waterNeeded;

	if (oxygenAvailable >= oxygenNeeded) oxygenRatio = 1;
	else oxygenRatio = oxygenAvailable / oxygenNeeded;

	if (sugarAvailable >= sugarNeeded) sugarRatio = 1;
	else sugarRatio = sugarAvailable / sugarNeeded;
	ResourceRequest* request;
	for (int i = 0; i < resourcesRequests.size(); i++) {
		request = resourcesRequests[i];
		
		if (color == request->cell->getColor() or not hasRole("antiparasit")) {
			request->water *= waterRatio;
			request->oxygen *= oxygenRatio;
			request->sugar *= sugarRatio;
		}
		else {
			request->water = 0;
			request->oxygen = 0;
			request->sugar = 0;
		}
	}
}

void Cell::distributeResources() {
	ResourceRequest* request;
	for (int i = 0; i < resourcesRequests.size(); i++) {
		request = resourcesRequests[i];
		request->cell->addWater(request->water);
		request->cell->addOxygen(request->oxygen);
		request->cell->addSugar(request->sugar);
		consumeWater(request->water);
		consumeOxygen(request->oxygen);
		consumeSugar(request->sugar);
		delete request;
	}
	resourcesRequests.clear();
}

float Cell::getElectricCharge() { 
	return electricCharge;
}

void Cell::addElectricPower(float power) {
	electricPower += power;
}

void Cell::setElectricPower(float power) {
	electricPower = power;
}

float Cell::getElectricPower() {
	return electricPower;
}

sf::Color Cell::getColor() {
	return color;
}

std::vector<Cell*> Cell::getLinkedNeighbors() {
	return linkedNeighbors;
}