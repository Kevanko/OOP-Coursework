#include "player.hpp"
#include "ship.hpp"

#include <time.h>

Player * Player::ActivePlayer = nullptr;

Player* Player::getActivePlayer()
{
	return ActivePlayer;
}

void Player::setActivePlayer(Player* player)
{
	if(ActivePlayer)
		ActivePlayer->grid->setGridActive(0);
	ActivePlayer = player;
	player->grid->setGridActive(1);
}

Player::Player(string name, float x, float y) : name(name)
{
	grid = new Grid(x, y, 400, 400);
	grid->setName(name);
	victories = 0;
	srand(time(NULL));
	placeShipsRandom();
}

Grid* Player::getGrid()
{
	return grid;
}

void Player::display(RenderWindow* win)
{
	updateShips();
	grid->draw(win);
}

Player::~Player()
{
	delete grid;
}

void Player::placeShip(Ship* ship, int size)
{
	for (int j = 0; j < size; j++)
	{
		Vector2i nextPos = Ship::GetCellCordsByDirection(ship, j);
		int id = nextPos.x + nextPos.y;
		ship->addCell(grid->cells[id]); 
		grid->cells[id]->setState(Cell::SHIP);
		grid->setClosedArround(nextPos);
	}
}

int Player::getShipsCount()
{
	return ships.size();
}

void Player::updateShips()
{
	for (int i = 0; i < ships.size(); i++) {
		if (ships[i].getAlive() == false) {
			destroyArrounsShip(&ships[i]);
			ships.erase(ships.begin() + i);
		}
	}
	if(ships.size() == 0)
		grid->setAliveAll(false);
}

string Player::getName()
{
	return name;
}

void Player::resetShips()
{
	clearShips();
	grid->setStatesAll(Cell::OPENED);
	grid->setAliveAll(true);
	placeShipsRandom();
}

void Player::placeShipsRandom()
{
	int delta = 0;
	int shipSize = 0;

	for (int i = 0; i < 10; i++) {
		if (ships.size() >= 1) delta = 1;
		if (ships.size() >= 3) delta = 2;
		if (ships.size() >= 6) delta = 3;
		shipSize = 4 - delta;

		Ship::DIRECTION direction = Ship::DIRECTION(rand() % 4);
		Ship ship(direction);
		Vector2i pos;

		int dx = shipSize * (ship.direction == Ship::RIGHT);
		int dy = shipSize * (ship.direction == Ship::BOTTOM);

		pos.x = rand() % (10 - dx) + dx * (ship.direction == Ship::LEFT);
		pos.y = rand() % (10 - dy) + dy * (ship.direction == Ship::TOP);

		if (!isValidPlace({ pos.x, pos.y }, ship.direction, shipSize))
		{
			bool valid = false;
			for (int i = 0; i < 4; i++) {
				ship.direction = Ship::DIRECTION(i);
				if (isValidPlace({ pos.x, pos.y }, ship.direction, shipSize)) {
					valid = true;
					break;
				}
			}
			if (!valid) {
				pos = findPlace(ship.direction, shipSize);
			}
		}

		ship.setStartPos({ pos.x, pos.y });
		placeShip(&ship, shipSize);
		ships.push_back(ship);
	}
}

void Player::destroyArrounsShip(Ship* ship)
{
	for (int i = 0; i < ship->getSize(); i++) {
		Vector2i pos = Ship::GetCellCordsByDirection(ship, i);
		grid->setDestroyedArround(pos);
	}
}

void Player::clearShips()
{
	int size = ships.size();
	for (int i = 0; i < size; i++) {
		ships.pop_back();
	}
}

Vector2i Player::findPlace(Ship::DIRECTION direction, int size) 
{
	for (int x = 2; x < 10; x += 3) {
		for (int y = 0; y < 10; y += 3) {
			if (isValidPlace({ x, y }, direction, size))
				return { x, y };
		}
	}
	for (int x = 0; x < 10; x++) {
		for (int y = 0; y < 10; y++) {
			if (isValidPlace({ x, y }, direction, size))
				return { x, y };
		}
	}

	return { 0, 0 };
}


bool Player::isValidPlace(Vector2i pos, Ship::DIRECTION direction, int size)
{
	switch (direction)
	{
	case Ship::LEFT:
		if (pos.y < size) return false;
		break;
	case Ship::RIGHT:
		if ((pos.y + size) > 10) return false;
		break;
	case Ship::TOP:
		if (pos.x < size) return false;
		break;
	case Ship::BOTTOM:
		if ((pos.x + size) > 10) return false;
		break;
	default:
		break;
	}

	for (int i = 0; i < size; i++) {
		Ship ship(direction);
		ship.setStartPos({ pos.x, pos.y });
		Vector2i nextPos = Ship::GetCellCordsByDirection(&ship, i);
		int id = nextPos.x + nextPos.y;
		if (grid->cells[id]->getState() != Cell::STATES::OPENED) return false;
	}

	return true;
}
