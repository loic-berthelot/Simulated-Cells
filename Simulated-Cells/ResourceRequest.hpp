#pragma once

class Cell;

struct ResourceRequest {
	Cell* cell;
	float water;
	float oxygen;
	float sugar;
	ResourceRequest(Cell* _cell) {
		cell = _cell;
	}
	ResourceRequest(Cell* _cell, float _water, float _oxygen, float _sugar) {
		cell = _cell;
		water = _water;
		oxygen = _oxygen;
		sugar = _sugar;
	}
};