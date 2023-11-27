#include "ship.hpp"

Ship::Ship() : Ship(RIGHT)
{
}

Ship::Ship(DIRECTION direction) : direction(direction)
{
	alive = false;
}

void Ship::setStartPos(Vector2i startPos)
{
	this->startPos = startPos;
}

Vector2i Ship::getStartPos()
{
	return startPos;
}

bool Ship::addCell(Cell* cell)
{
	if (ship.size() < 4) {
		ship.push_back(cell);
		return 0;
	}
	return 1;
}

bool Ship::resetShip()
{
	int size = ship.size();
	for (int i = 0; i < size; i++) {
		ship.pop_back();
	}
	if (ship.size() == 0)
		return 0;
	return 1;
}

bool Ship::getAlive()
{
	update();
	return alive;
}

Vector2i Ship::GetCellCordsByDirection(Ship *ship, int id)
{
	int y = ship->startPos.y + id * (ship->direction == Ship::RIGHT) - id * (ship->direction == Ship::LEFT);
	int x = (ship->startPos.x + id * (ship->direction == Ship::BOTTOM) - id * (ship->direction == Ship::TOP)) * 10;

	return {x, y};
}

int Ship::getSize()
{
	return ship.size();
}

void Ship::update()
{
	for (auto& cell : ship) {
		if (cell->getAlive()) {
			alive = true;
			return;
		}
	}
	alive = false;
}


