#include "commons.hpp"
#include "InfoPanel.hpp"
#include "Button.hpp"
/**
* 
* 
* 
* 
* 
* 
*/




// Variables globales

extern int windowWidth;
extern int windowHeight;
sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Simulated Cells", sf::Style::Fullscreen);
sf::Vector2f SIZE;
float zoom; // size of the map
sf::Vector2f camPos;

bool pause;
bool displayQuadTree;
bool fastMode;
bool fullscreen;
bool displayLinks;
bool nightEnabled;

QuadTree* Q;
Environment* E;
InfoPanel* infoPanel;
Cell* selectedCell;
sf::Clock clockTime;
int fps;
std::string mapName;
int cellsNumber;
int initialMutations;
sf::Vector2i mouse;
bool clic;

Button* button_fastMode;
Button* button_pause;
Button* button_quadTree;
Button* button_fullscreen;
Button* button_displayLinks;
Button* button_nightEnabled;

void init() {
	//parameters
	SIZE = sf::Vector2f(1000, 1000);
	mapName = "background5";
	cellsNumber = 200;
	initialMutations = 8;

	srand(time(0));
	zoom = 700/std::max(SIZE.x, SIZE.y);
	camPos = multiplyVector(SIZE, 0.5);
	fastMode = false;
	pause = false;
	displayQuadTree = false;
	fullscreen = true;
	displayLinks = false;
	nightEnabled = true;

	RectByCenter bd(multiplyVector(SIZE, 0.5), multiplyVector(SIZE, 0.5));
	if (Q != nullptr) Q->del();
	delete(Q);
	Q = new QuadTree(bd);
	delete(E);
	E = new Environment(Q, SIZE, mapName);
	infoPanel = new InfoPanel(sf::Vector2f(windowWidth, windowHeight), E);

	RectByCenter rect;
	rect.center = sf::Vector2f(SIZE.x / 2.0, SIZE.y / 2.0);
	rect.radius = sf::Vector2f(SIZE.x / 2.0, SIZE.y / 2.0);
	QuadTree quadTree(rect);
	selectedCell = nullptr;
	window.setPosition(sf::Vector2i(-10, 0));

	button_fullscreen = new Button("fullscreen", sf::Vector2f(windowWidth - 30, 30));
	button_pause = new Button("pause", sf::Vector2f(20, windowHeight - 100));
	button_displayLinks = new Button("displayLinks", sf::Vector2f(55, windowHeight - 100));
	button_fastMode = new Button("fastMode", sf::Vector2f(90, windowHeight - 100));
	button_quadTree = new Button("quadTree", sf::Vector2f(125, windowHeight - 100));
	button_nightEnabled = new Button("nightEnabled", sf::Vector2f(160, windowHeight - 100));
}


void generate(sf::RenderWindow& window) {
	DNA* dna;
	for (int i = 0; i < cellsNumber; i++) {
		std::string text;
		//text = "fat,link5; ; little,noLink";
		//text = "childIndex_0_1; fat; muscle; poison";
		//text = "photosynthesis, fat, poison, lifetime300; photosynthesis, childIndexNull, childIndex_0_1; lung, fat, childIndex_0_1; water_reproducer, fat, lifetime300; gill; fat, childIndex_2";
		//text = "photosynthesis, muscle, noLink, link1; mediumClock, glue, noLink, link1";
		//text = "photosynthesis, slowClock, childIndex_0_1; explosive, electricitySensitive; noLink, link1";
		text = "photosynthesis";
		dna = new DNA(text);
		for (int i = 0; i < initialMutations; i++) dna = new DNA(dna, 0.2);
		Cell* NewCell = new Cell(sf::Vector2f(rand() % (int)SIZE.x, rand() % (int)SIZE.y), E, dna);
		//Cell* NewCell = new Cell(sf::Vector2f(480, 600), E, dna);
		E->addCell(NewCell);
	}

	sf::RenderTexture mapTexture;
	mapTexture.create(SIZE.x, SIZE.y);

	sf::RectangleShape map;
	map.setSize(multiplyVector(SIZE, zoom));
	map.setPosition(sf::Vector2f(0, 0));

	RectByCenter captureZone;

	while (true) {
		mouse = sf::Mouse::getPosition(window);
		clic = sf::Mouse::isButtonPressed(sf::Mouse::Left);
		sf::Event event;
		if (fastMode) window.setFramerateLimit(6000);
		else window.setFramerateLimit(6000);
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) { window.close(); return; }
			else if (event.type == sf::Event::Resized) {
				// update the view to the new size of the window
				sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
				window.setView(sf::View(visibleArea));
				windowWidth = event.size.width;
				windowHeight = event.size.height;
			} 

			else if (event.type == sf::Event::MouseWheelMoved) {
				int mdt = std::max(-1, std::min(event.mouseWheel.delta, 1));
				zoom *= 1 + ((float)mdt) / 15;

				map.setSize(multiplyVector(SIZE, zoom));				
			}			
			
			
			else if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Up)	camPos.y -= 20/zoom;
				if (event.key.code == sf::Keyboard::Down) camPos.y += 20/zoom;
				if (event.key.code == sf::Keyboard::Left) camPos.x -= 20/zoom;
				if (event.key.code == sf::Keyboard::Right) camPos.x += 20/zoom;
			}

			else if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					if (mouse.x > 200) {
						std::vector<Cell*> cellsSelected;
						cellsSelected.clear();
						float range = 100;

						sf::Vector2f shift = addVectors(multiplyVector(camPos, -zoom), sf::Vector2f(windowWidth / 2, windowHeight / 2));	
						sf::Vector2f relativeMousePos = sf::Vector2f((mouse.x - shift.x) / zoom, (mouse.y - shift.y) / zoom);
						captureZone.center = relativeMousePos;
						captureZone.radius = sf::Vector2f(range, range);
						E->Quad->queryRangeCircle(captureZone, cellsSelected);
						selectedCell = nullptr;
						float dist, minDist;
						minDist = 2*range*range;
						for (int i = 0; i < cellsSelected.size(); i++) {
							sf::Vector2f cellPos = cellsSelected[i]->getPosition();
							float radius = cellsSelected[i]->getRadius();
							if (pointInFrame(relativeMousePos, sf::IntRect(cellPos.x - radius, cellPos.y - radius, 2 * radius, 2 * radius))) {
								dist = distanceBetweenPoints(cellPos, relativeMousePos);
								if (dist < minDist and dist <= radius) {
									minDist = dist;
									selectedCell = cellsSelected[i];
								}
							}
						}
						infoPanel->updateCell(selectedCell);
					}
					
				}
			}
		}


		if (not pause) {
			E->frameNumber++;
			if (E->Quad != nullptr) E->Quad->del();
			int c = 0; 
			while(c < E->cells.size()){ 
				if (E->cells[c]->isDead()) {
					delete E->cells[c]; // On supprime la cellule...
					E->cells.erase(E->cells.begin()+c); // ... puis on supprime la "case" du std::vector correspondante
				}
				else {
					E->Quad->insert(E->cells[c]);
					c++;
				}
			}

			E->evolve();
			if (selectedCell != nullptr) {
				if (selectedCell->isDead()) {
					selectedCell = nullptr;
					infoPanel->updateCell(nullptr);
				}
			}
		}
		if (selectedCell == nullptr) {
			E->updateCellFocus(sf::IntRect(0, 0, 0, 0));
		}
		else {
			sf::Vector2f pos = selectedCell->getPosition();
			float radius = selectedCell->getRadius();
			E->updateCellFocus(sf::IntRect(pos.x - radius - 5, pos.y - radius - 5, 2 * radius + 10, 2 * radius + 10));
		}
		if (pause) fps = 0; 
		else fps = (int)1.f / clockTime.restart().asSeconds();
		
		//display
		window.clear();
		if (not fastMode or E->frameNumber%100==1 or pause) {	
			mapTexture.clear();		
			E->display(mapTexture, displayLinks, nightEnabled);
			if (displayQuadTree) Q->display(mapTexture);
			map.setPosition(addVectors(multiplyVector(camPos, -zoom), sf::Vector2f(windowWidth/2, windowHeight/2)));
			map.setTexture(&mapTexture.getTexture());
			mapTexture.display();
			
		}
		window.draw(map);
		infoPanel->display(&window, fps, fastMode, nightEnabled);
		if (button_fastMode->interact(mouse, clic)) fastMode = not fastMode;
		if (button_pause->interact(mouse, clic)) pause = not pause;
		if (button_quadTree->interact(mouse, clic)) displayQuadTree = not displayQuadTree;
		if (button_fullscreen->interact(mouse, clic)) {
			fullscreen = not fullscreen; 
			if (fullscreen) window.create(sf::VideoMode(windowWidth, windowHeight), "Simulated Cells", sf::Style::Fullscreen);
			else window.create(sf::VideoMode(windowWidth, windowHeight), "Simulated Cells");
		}
		if (button_displayLinks->interact(mouse, clic)) displayLinks = not displayLinks;
		if (button_nightEnabled->interact(mouse, clic)) nightEnabled = not nightEnabled;
		button_fullscreen->display(&window);
		button_fastMode->display(&window);
		button_pause->display(&window);
		button_quadTree->display(&window);
		button_displayLinks->display(&window);
		button_nightEnabled->display(&window);
		window.display();
	}
}


int main() {
	init();
	generate(window);
}
