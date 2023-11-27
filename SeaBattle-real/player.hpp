#pragma once

#include <SFML/Graphics.hpp>

#include "object.hpp"
#include "ship.hpp"


class Player {
public:
	friend class Game;

	static Player* getActivePlayer();
	static void setActivePlayer(Player * player);
	void placeShip(Ship* ship, int size);
	int getShipsCount();
	void updateShips();
	string getName();
	void resetShips();

	Player(string name, float x, float y);
	Grid * getGrid();
	void display(RenderWindow* win);

	~Player();

private:
	void placeShipsRandom();
	void destroyArrounsShip(Ship* ship);
	void clearShips();
	Vector2i findPlace(Ship::DIRECTION direction, int size);
	bool isValidPlace(Vector2i pos, Ship::DIRECTION direction, int size);

	string name;
	Texture texture;
	int victories;
	static Player* ActivePlayer;
	vector<Ship> ships;
	Grid* grid;
};