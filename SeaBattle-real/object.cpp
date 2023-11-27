#include <vector>
#include <SFML/Graphics.hpp>

#include "object.hpp"
#include "ship.hpp"

void BgGrid::drawLines()
{
	Vector2f size = bg.getSize();
	Vector2f pos = bg.getPosition();

	Color color(125, 200, 255, 255);

	for (int i = 0; i < size.x / interval; i++) {
		RectangleShape line({ 1, size.y });
		line.setFillColor(color);
		line.setPosition({ pos.x + i * interval, pos.y });
		lines.push_back(line);
	}
	for (int i = 0; i < size.y / interval; i++) {
		RectangleShape line({ size.x, 1 });
		line.setFillColor(color);
		line.setPosition({ pos.x,  i * interval + pos.y });
		lines.push_back(line);
	}
	//RED LINE
	if (pos.x == 0 && pos.y == 0) {
		RectangleShape line({ size.x, 5 });
		line.setFillColor(Color(255, 0, 50, 155));
		line.setPosition({ 0, interval * 2.2f });
		lines.push_back(line);

	}
}

void BgGrid::clearLines()
{
	while (lines.size() > 0) 
	{
		lines.pop_back();
	}
}

/* BG grid */

BgGrid::BgGrid(float x, float y, float w, float h, float interval) : ShapeRect(x, y, w, h), interval(interval)
{
	drawLines();
}

void BgGrid::draw(RenderWindow* win)
{
	win->draw(bg);
	for (RectangleShape line : lines) {
		win->draw(line);
	}
}

void BgGrid::setTexture(Texture* texture)
{
	bg.setTexture(texture);
	clearLines();
	drawLines();
}

/* GAME cell */
Cell::Cell() : Cell(0, 0, 10, 10)
{
}

Cell::Cell(float x, float y, float w, float h) : ShapeRect(x + 1, y + 1, w - 1, h - 1)
{
	block_state = OPENED;
	alive = true;
	bg.setOutlineThickness(1);
	bg.setOutlineColor(Color::Red);
}

void Cell::setState(STATES st)
{
	block_state = st;
}

void Cell::setAlive(bool a)
{
	alive = a;
}


bool Cell::getAlive()
{
	return alive;
}


Cell::STATES Cell::getState()
{
	return block_state;
}

/* GAME grid */

Grid::Grid(float x, float y, float w, float h) : BgGrid(x, y, w + 1, h + 1, 40)
{
	showShips = false;
	bg.setOutlineThickness(2);
	bg.setOutlineColor(Color::Black);
	position = { x, y };

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			float width = w / 10;
			float height = h / 10;
			Cell* b = new Cell(x + width * j, y + height * i, width, height);
			b->setObjColor(Color(0, 0, 0, 0));
			cells.push_back(b);
		}
	}

	text_left = text_top = Text("", font, 29);
	text_top.setString("ABCDEFGHIJ");
	text_top.setLetterSpacing(4.8);
	text_top.setFillColor(Color(50, 25, 255, 225));
	text_top.setStyle(Text::Bold);
	text_top.setPosition({ x + 5, y - 40 });

	text_left.setString(" 1\n 2\n 3\n 4\n 5\n 6\n 7\n 8\n 9\n10");
	text_left.setLineSpacing(1.18);
	text_left.setFillColor(Color(50, 25, 255, 225));
	text_left.setStyle(Text::Bold);
	text_left.setPosition({ x - 42, y });

	name.setFont(font);
	name.setString("NULL");
	name.setFillColor(Color(25, 25, 60, 200));
	name.setCharacterSize(35);
	name.setPosition({ x + 200 - name.getLocalBounds().width / 2, y - 100});

}

void Grid::setGridActive(bool active)
{
	if(active)
		bg.setOutlineColor(Color::Red);
	else
		bg.setOutlineColor(Color::Black);
}

void Grid::resetGrid()
{
	for (auto& cell : cells) {
		cell->setAlive(true);
		cell->setState(Cell::OPENED);
	}
}

void Grid::setAliveAll(bool v)
{
	for (auto& cell : cells) {
		cell->setAlive(v);
	}
}

void Grid::setStatesAll(Cell::STATES st)
{
	for (auto& cell : cells) {
		cell->setState(st);
	}
}

void Grid::setClosedArround(Vector2i pos)
{
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			int x = (pos.x + i * 10);
			int y = pos.y + j;
			int id = x + y;
			if (x < 0 || x > 99 || y < 0 || y > 9) continue;
			if (cells[id]->getState() == Cell::OPENED)
				cells[id]->setState(Cell::CLOSED);
		}
	}
}

void Grid::setDestroyedArround(Vector2i pos)
{
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			int x = (pos.x + i * 10);
			int y = pos.y + j;
			int id = x + y;
			if (x < 0 || x > 99 || y < 0 || y > 9) continue;
			cells[id]->setAlive(false);

		}
	}
}

void Grid::setShowShips(bool v)
{
	showShips = v;
}

void Grid::draw(RenderWindow* win)
{
	Cell* active = activeCell;
	BgGrid::draw(win);

	for (auto& cell : cells) {
		if (active == cell)
			continue;

		if (cell->getState() == Cell::SHIP && showShips)
			cell->setObjColor(Color(50, 200, 50, 200));

		else if(cell->getAlive() == false && cell->getState() != Cell::SHIP && !showShips)
			cell->setObjColor(Color(50, 50, 50, 125));

		else if (cell->getAlive() == false && cell->getState() == Cell::SHIP && !showShips)
			cell->setObjColor(Color(200, 0, 50, 200));

		else 
			cell->setObjColor(Color(0, 0, 0, 0));


		cell->setOutlineThickness(0);
		cell->draw(win);
	}
	if (active) {
		active->setOutlineThickness(1);

		active->draw(win);
	}

	win->draw(text_top);
	win->draw(text_left);
	win->draw(name);
}

Grid::~Grid()
{
	for (auto& cell : cells) {
		delete cell;
	}
}

void Grid::setName(string name)
{
	this->name.setString(name);
	this->name.setPosition({ position.x + 200 - this->name.getLocalBounds().width / 2, position.y - 100 });
}

bool Grid::handleClick(Grid* grid, RenderWindow* win) {
	Vector2f mousePosition = win->mapPixelToCoords(Mouse::getPosition(*win));
	
	for (auto& cell : grid->cells) {
		if (cell->contains(mousePosition) && cell->getAlive()) {
			grid->activeCell = nullptr;
			if(!grid->showShips)
				cell->setAlive(false);
			if (cell->getState() == Cell::SHIP) {
				return 0;
			}
			return 1;
		}
	}
	return 0;
}

bool Grid::handleHover(Grid* grid, RenderWindow* win) {
	Vector2f mousePosition = win->mapPixelToCoords(Mouse::getPosition(*win));
	for (auto& cell : grid->cells) {
		if (cell->contains(mousePosition) && cell->getAlive()) {
			grid->activeCell = cell;
			return 1;
		}
		else {
			grid->activeCell = nullptr;
		}
	}
	return 0;
}

ShapeRect::ShapeRect(int x, int y, int w, int h)
{

	if (!font.loadFromFile("ofont.ru_Maki Sans.ttf"))
	{

	}

	bg = RectangleShape(Vector2f(w, h));
	bg.setPosition(x, y);
}

bool ShapeRect::contains(Vector2f point)
{
	return bg.getGlobalBounds().contains(point);
}

void ShapeRect::setOutlineColor(Color color)
{
	bg.setOutlineColor(color);
}

void ShapeRect::setOutlineThickness(float thickness)
{
	bg.setOutlineThickness(thickness);
}


void ShapeRect::setObjColor(Color color)
{
	bg.setFillColor(color);
}


void ShapeRect::setTexture(Texture* texture)
{
	bg.setTexture(texture);
}

void ShapeRect::draw(RenderWindow* win)
{
	win->draw(bg);
}

Button::Button(string name, string st, Vector2f pos, Vector2f size, Color color, float fontSize) : ShapeRect(pos.x, pos.y, size.x, size.y), color(color), name(name)
{
	active = true;
	bg.setFillColor(Color(25, 25, 225, 0));
	bg.setPosition(pos.x, pos.y);

	this->text.setCharacterSize(fontSize);
	this->text.setFillColor(color);
	this->text.setFont(font);
	this->text.setString(st);

	float cX = pos.x + size.x / 2 - this->text.getLocalBounds().width / 2 - 5;
	float cY = pos.y + size.y / 2 - this->text.getLocalBounds().height / 2 - 8;

	this->text.setPosition({ cX, cY});

}

void Button::draw(RenderWindow* win)
{
	win->draw(bg);
	win->draw(this->text);
}

string Button::getName()
{
	return name;
}

string Button::getString()
{
	return text.getString();
}

void Button::setString(string st)
{
	text.setString(st);
}

void Button::setTextColor(Color color)
{
	text.setFillColor(color);
}

bool Button::handleClick(Button* button, RenderWindow* win)
{
	Vector2f mousePosition = win->mapPixelToCoords(Mouse::getPosition(*win));

	if (button->contains(mousePosition) && button->active) {
		button->setTextColor(button->color);
		return true;
	}
	return false;
}

bool Button::handleHover(Button* button, RenderWindow* win)
{
	Vector2f mousePosition = win->mapPixelToCoords(Mouse::getPosition(*win));

	if (button->contains(mousePosition) && button->active) {
		button->setTextColor(Color::Green);
		return 1;
	}
	button->setTextColor(button->color);
	return 0;
}

bool Button::getActive()
{
	return active;
}

void Button::setActive(bool a)
{
	active = a;
}
