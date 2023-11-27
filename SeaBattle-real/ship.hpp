#pragma once

#include <vector>

#include "object.hpp"


class Ship {
public:
	friend class Player;
	enum DIRECTION {
		LEFT,
		TOP,
		RIGHT,
		BOTTOM
	};

	Ship();
	Ship(DIRECTION direction);
	void setStartPos(Vector2i startPos);
	Vector2i getStartPos();
	bool addCell(Cell* cell);
	bool resetShip();
	bool getAlive();
	static Vector2i GetCellCordsByDirection(Ship *ship, int id);
	int getSize();

private:
	void update();
	Vector2i startPos;
	vector<Cell*> ship;
	DIRECTION direction;
	bool alive;
};