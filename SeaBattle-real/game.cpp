#include <SFML/Graphics.hpp>
#include <iostream>

#include "game.hpp"
#include "object.hpp"
#include "player.hpp"


Game::Game() : Game(1280, 720, "SeaBattle")
{
}

Game::Game(int w, int h, const char* title)
{
	Color btnColor = Color::Blue;
	Color textColor = Color(25, 25, 50, 200);
	cur_h.loadFromSystem(Cursor::Hand);
	cur_a.loadFromSystem(Cursor::Arrow);

	playersReady = 0;
	status = P1SHIPS;
	int blockSize = 40;
	win = new RenderWindow(VideoMode(w, h), title, Style::Default);
	if (win->getSize().x < 1000 || win->getSize().x < 600) {
		win->setSize({ 1000, 600 });
	}
	bg = new  BgGrid(0, 0, win->getSize().x, win->getSize().y, blockSize);
	moves = 0;

	int maxBlocksX = win->getSize().x / blockSize;
	int maxBlocksY = win->getSize().y / blockSize;
	int centerX = maxBlocksX / 4 - 5;
	int centerY = maxBlocksY / 1.8 - 5;

	player1 = new Player("Player1", (centerX)*blockSize, centerY * blockSize);
	player2 = new Player("Player2", (maxBlocksX - centerX - 10) * blockSize, centerY * blockSize);

	arrow = ConvexShape(3);

	arrow.setPoint(0, sf::Vector2f(0.f, -40.f));
	arrow.setPoint(1, sf::Vector2f(40.f, 40.f));
	arrow.setPoint(2, sf::Vector2f(-40.f, 40.f));

	arrow.setOrigin({ 0.f, 0.f });
	arrow.setFillColor(Color(0, 0, 0, 200));
	arrow.setPosition({ (maxBlocksX / 2.f) * 40, (maxBlocksY / 2.f + 1) * 40});


	buttons.push_back(new Button("Start", "Play", {maxBlocksX / 2 * 40.f - 80, centerY * 40.f + 400}, {160, 80}, btnColor, 45));
	buttons.push_back(new Button("Ready", "Ready", {maxBlocksX / 2 * 40.f - 80, centerY * 40.f + 400}, {160, 80}, btnColor, 45));
	buttons.push_back(new Button("Next", "Next", {maxBlocksX / 2 * 40.f - 80, centerY * 40.f + 400}, {160, 80}, btnColor, 45));
	buttons.push_back(new Button("Exit", "Exit", {(maxBlocksX - 4.f) * 40, 0}, {160, 80}, btnColor, 45));
	buttons.push_back(new Button("Random P1", "Rand", {float(centerX + 4) * blockSize, centerY * 40.f + 400.f}, {80, 40}, btnColor, 25));
	buttons.push_back(new Button("Random P2", "Rand", { (maxBlocksX - centerX - 6.f)* blockSize, centerY * 40.f + 400.f }, {80, 40}, btnColor, 25));
	buttons.push_back(new Button("Counter Victories", "0 : 0", { float(maxBlocksX / 2 - 1) * blockSize, centerY * 40.f + 40.f }, { 80, 40 }, textColor, 25));
	buttons.push_back(new Button("P1 Ships", "ships remained: 10", { float(centerX) * blockSize, centerY * 40.f + 400.f }, { 400, 40 }, textColor, 25));
	buttons.push_back(new Button("P2 Ships", "ships remained: 10", { (maxBlocksX - centerX - 10.f) * blockSize, centerY * 40.f + 400.f }, { 400, 40 }, textColor, 25));
	buttons.push_back(new Button("Active Player", "Attacked player: None", { 0, 40.f }, { float(win->getSize().x), 40.f}, textColor, 25));

	Player::setActivePlayer(player1);
	win->setFramerateLimit(60);
}

void Game::start()
{
	Clock clc;
	while (win->isOpen()) {
		int elapced = clc.restart().asMilliseconds();
		update(elapced);

		draw();
	}
}

Game::~Game()
{
	delete win;
	delete player1;
	delete player2;
	delete bg;

	for (auto& button : buttons) {
		delete button;
	}

}

void Game::update(int elapced)
{
	Event ev;
	while (win->pollEvent(ev)) {
		if (ev.type == Event::Closed) {
			win->close();
		}
		if (ev.type == Event::MouseButtonPressed) {
			if (ev.mouseButton.button == Mouse::Left) {

				if (Grid::handleClick(Player::getActivePlayer()->getGrid(), win)) {
					win->setMouseCursor(cur_a);
					if(status == PLAYING)
						moves++;
				}

				for (auto& btn : buttons) {
					if (Button::handleClick(btn, win)) {
						if(btn->getName() == "Exit")
							win->close();
						if (btn->getName() == "Ready") {
							moves++;
							playersReady++;
						}
						if (btn->getName() == "Next") {
							moves = 0;
							playersReady = 0;
							status = P1SHIPS;
							player1->resetShips();
							player2->resetShips();
						}
						if (btn->getName() == "Random P1") {
							player1->resetShips();
						}
						if (btn->getName() == "Random P2") {
							player2->resetShips();
						}
						win->setMouseCursor(cur_a);
						break;
					}
				}
			}
		}
		if (ev.type == Event::MouseMoved) {
			bool setCursor = false;

			for (auto& btn : buttons) {
				setCursor = true;
				if (Button::handleHover(btn, win)) {
					win->setMouseCursor(cur_h);
					setCursor = false;
					break;
				}

			}
			if (setCursor) {
				win->setMouseCursor(cur_a);
			}

			if (Grid::handleHover(Player::getActivePlayer()->getGrid(), win)) {
				win->setMouseCursor(cur_h);
			}
		}
	}
}

void Game::draw()
{
	win->clear();

	bg->draw(win);

	if (moves % 2 == 0) {
		Player::setActivePlayer(player1);
	}
	else {
		Player::setActivePlayer(player2);
	}

	if (playersReady == 1) {
		status = P2SHIPS;
	}
	if (playersReady == 2) {
		playersReady = 0;
		status = READY;
	}
	switch (status) {

	case P1SHIPS:
		player1->grid->setShowShips(true);
		player2->grid->setShowShips(false);
		break;

	case P2SHIPS:
		player2->grid->setShowShips(true);
		player1->grid->setShowShips(false);
		break;

	case READY:
		player1->grid->setShowShips(false);
		player2->grid->setShowShips(false);
		status = PLAYING;
		break;

	case PLAYING:
		if (player1->getShipsCount() == 0) {
			player2->grid->setShowShips(true);
			player2->grid->setAliveAll(true);
			player2->victories += 1;
			status = END;
		}
		else if (player2->getShipsCount() == 0) {
			player1->grid->setShowShips(true);
			player1->grid->setAliveAll(true);
			player1->victories += 1;
			status = END;
		}
		break;

	case END:
		break;
	default:
		break;
	}

	player1->display(win);
	player2->display(win);

	for (auto& button : buttons) {
		if (button->getName() == "Ready" && (status == P1SHIPS || status == P2SHIPS)) {
			button->setActive(true);
			button->draw(win);
		}

		else if (button->getName() == "Start" && status == READY) {
			button->setActive(true);
			button->draw(win);
		}

		else  if (button->getName() == "Next" && status == END) {
			button->setActive(true);
			button->draw(win);
		}

		else if (button->getName() == "Exit") {
			button->setActive(true);
			button->draw(win);
		}
		else if (button->getName() == "Random P1" && status == P1SHIPS) {
			button->setActive(true);
			button->draw(win);
		}
		else if (button->getName() == "Random P2" && status == P2SHIPS) {
			button->setActive(true);
			button->draw(win);
		}
		else if (button->getName() == "Counter Victories") {
			string st = to_string(player1->victories) + " : " + to_string(player2->victories);
			button->setString(st);
			button->setActive(false);
			button->draw(win);
		}
		else if (button->getName() == "P1 Ships" && status == PLAYING) {
			string st = "ships remained: " + to_string(player1->getShipsCount());
			button->setString(st);
			button->setActive(false);
			button->draw(win);
		}
		else if (button->getName() == "P2 Ships" && status == PLAYING) {
			string st = "ships remained: " + to_string(player2->getShipsCount());
			button->setString(st);
			button->setActive(false);
			button->draw(win);
		}
		else if (button->getName() == "Active Player" && status == PLAYING) {
			string st = "Attacked player: " + Player::getActivePlayer()->getName();
			button->setString(st);
			button->setActive(false);
			button->draw(win);
		}
		else button->setActive(false);
	}

	if (status == PLAYING) {
		if (Player::getActivePlayer() == player1) {
			arrow.setRotation(-90);
		}
		if (Player::getActivePlayer() == player2) {
			arrow.setRotation(90);
		}
		win->draw(arrow);
	}

	win->display();
}
