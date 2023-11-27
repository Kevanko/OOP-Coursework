#pragma once

#include <SFML/Graphics.hpp>

#include "object.hpp"
#include "player.hpp"

using namespace sf;
using namespace std;

class Game {
public:
	enum STATUS {
		P1SHIPS,
		P2SHIPS,
		READY,
		PLAYING,
		END
	};
	Game();
	Game(int w, int h, const char* title);
	void start();
	~Game();

private:
	RenderWindow* win;
	BgGrid* bg;
	Player* player1, * player2;
	int moves;
	STATUS status;
	Cursor cur_h, cur_a;

	ConvexShape arrow;
	int playersReady;
	vector<Button*> buttons;
	void draw();
	void update(int elapced);
};
