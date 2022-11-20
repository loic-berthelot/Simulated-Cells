#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <time.h>

void resizeImage(const sf::Image& originalImage, sf::Image& resizedImage);

float sqrt2(float x, float dx);

void write(sf::RenderWindow* window, std::string t1, sf::Vector2f position /*top left angle*/, sf::Vector2f maxSize, sf::Color color);

sf::Vector2f addVectors(sf::Vector2f v1, sf::Vector2f v2);
sf::Vector2f substractVectors(sf::Vector2f v1, sf::Vector2f v2);
sf::Vector2f multiplyVector(sf::Vector2f v, float f);

float dotProduct(sf::Vector2f v1, sf::Vector2f v2);
float vectorNorm(sf::Vector2f v, float precision = 0.01);
sf::Vector2f normalizeVector(sf::Vector2f, float f);

bool pointInFrame(sf::Vector2f v, sf::FloatRect r, float radius = 0);
bool pointInFrame(sf::Vector2f v, sf::IntRect r, float radius = 0);

void nextClicState(int& clicState, bool clicked, bool touched);

float distanceBetweenPoints(sf::Vector2f v1, sf::Vector2f v2, float precision = 0.01);

#include "CellModel.hpp"
#include "DNA.hpp"
#include "RectByCenter.hpp"
#include "Cell.hpp"
#include "QuadTree.hpp"
#include "CellFocus.hpp"
#include "Environment.hpp"
