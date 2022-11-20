#include "commons.hpp"

int windowWidth = sf::VideoMode::getDesktopMode().width;
int windowHeight = sf::VideoMode::getDesktopMode().height;

float sqrt2(float x, float dx) {
	return sqrt(x);
	/*
	float a, b, avg;
	if (x > 1) {
		a = 1;
		b = x;
	}
	else {
		a = x;
		b = 1;
	}
	avg = (a + b) / 2;
	while (b - a > dx) {
		if (x < avg * avg) b = avg;
		else a = avg;
		avg = (a + b) / 2;
	}
	return avg;
	*/
}


void write(sf::RenderWindow* window, std::string t1, sf::Vector2f position /*top left angle*/, sf::Vector2f maxSize, sf::Color color) {
	//load font
	sf::Font font;
	font.loadFromFile("../fonts/BalooPaaji2.ttf");

	//create the text element
	sf::Text text(t1, font);
	text.setStyle(sf::Text::Bold);
	text.setFillColor(color);

	//size and place the text element according to the space available
	text.setCharacterSize(maxSize.y / 2);
	sf::FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

	text.setPosition(sf::Vector2f(position.x + maxSize.x / 2, position.y + maxSize.y / 2));

	//text.getLocalBounds();

	window->draw(text);
}



float dotProduct(sf::Vector2f v1, sf::Vector2f v2) { return v1.x * v2.x + v1.y * v2.y; }

float vectorNorm(sf::Vector2f v, float precision) { return sqrt2(v.x * v.x + v.y * v.y, precision); }

sf::Vector2f addVectors(sf::Vector2f v1, sf::Vector2f v2) { return sf::Vector2f(v1.x + v2.x, v1.y + v2.y); }

sf::Vector2f substractVectors(sf::Vector2f v1, sf::Vector2f v2) { return sf::Vector2f(v1.x - v2.x, v1.y - v2.y); }

sf::Vector2f multiplyVector(sf::Vector2f v, float f) { return sf::Vector2f(v.x * f, v.y * f); }

sf::Vector2f normalizeVector(sf::Vector2f v, float f = 1) { 
	if (v.x == 0 and v.y == 0) return normalizeVector(sf::Vector2f(rand()%201-100, rand() % 201 - 100), f);
	return multiplyVector(v, f/vectorNorm(v)); 
}


float distanceBetweenPoints(sf::Vector2f v1, sf::Vector2f v2, float precision) {
	return vectorNorm(substractVectors(v2, v1), precision);
}

bool pointInFrame(sf::Vector2f v, sf::FloatRect rect, float radius) {
	if (v.x - radius < rect.left || v.x + radius > rect.left + rect.width) return false;
	return v.y - radius >= rect.top && v.y + radius <= rect.top + rect.height;
}

bool pointInFrame(sf::Vector2f v, sf::IntRect rect, float radius) {
	if (v.x - radius < rect.left || v.x + radius > rect.left + rect.width) return false;
	return v.y - radius >= rect.top && v.y + radius <= rect.top + rect.height;
}

void nextClicState(int& clicState, bool clicked, bool touched) {
	if (clicState == 0) {
		if (touched and not clicked) clicState = 1;
	}
	else if (clicState == 1) {
		if (touched) {
			if (clicked) clicState = 2;
		}
		else clicState = 0;
	}
	else if (clicState == 2) {
		if (touched) {
			if (not clicked) clicState = 3;
		}
		else clicState = 0;
	}
	else if (clicState == 3) {
		if (touched) clicState = 1;
		else clicState = 0;
	}
}

void resizeImage(const sf::Image& originalImage, sf::Image& resizedImage)
{
	const sf::Vector2u originalImageSize{ originalImage.getSize() };
	const sf::Vector2u resizedImageSize{ resizedImage.getSize() };
	for (unsigned int y{ 0u }; y < resizedImageSize.y; ++y)
	{
		for (unsigned int x{ 0u }; x < resizedImageSize.x; ++x)
		{
			unsigned int origX{ static_cast<unsigned int>(static_cast<double>(x) / resizedImageSize.x * originalImageSize.x) };
			unsigned int origY{ static_cast<unsigned int>(static_cast<double>(y) / resizedImageSize.y * originalImageSize.y) };
			resizedImage.setPixel(x, y, originalImage.getPixel(origX, origY));
		}
	}
}
