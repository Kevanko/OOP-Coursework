#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Object {
public:
	virtual void draw(RenderWindow* win) = 0;
	virtual void setTexture(Texture* texture) = 0;
	virtual void setObjColor(Color color) = 0;
	virtual void setOutlineColor(Color color) = 0;
	virtual bool contains(Vector2f point) = 0;
	virtual void setOutlineThickness(float thickness) = 0;

	virtual ~Object() {};
};

class ShapeRect : public Object {
protected:
	RectangleShape bg;
	Font font;

public:
	ShapeRect(int x, int y, int w, int h);

	virtual bool contains(Vector2f point) override;
	virtual void draw(RenderWindow* win) override;
	virtual void setTexture(Texture* texture) override;
	virtual void setObjColor(Color color) override;
	virtual void setOutlineColor(Color color) override;
	virtual void setOutlineThickness(float thickness) override;
};

class Button : public ShapeRect {
public:
	Button(string name, string st, Vector2f pos, Vector2f size, Color color, float fontSize);
	virtual void draw(RenderWindow* win) override;
	string getName();
	string getString();
	void setString(string st);
	void setTextColor(Color color);
	static bool handleClick(Button* button, RenderWindow* win);
	static bool handleHover(Button* button, RenderWindow* win);
	bool getActive();
	void setActive(bool a);

private:
	Color color;
	bool active;
	string name;
	Text text;
};

class BgGrid : public ShapeRect {
public:
	BgGrid(float x, float y, float w, float h, float interval);

	virtual void draw(RenderWindow* win) override;
	virtual void setTexture(Texture* texture) override;

	void drawLines();
	void clearLines();


private:
	float interval;
	vector<RectangleShape> lines;
};

class Cell : public ShapeRect {
public:
	enum STATES 
	{
		OPENED,
		CLOSED,
		SHIP,
	};

	Cell();
	Cell(float x, float y, float w, float h);

	void setState(STATES st);
	STATES getState();
	void setAlive(bool a);
	bool getAlive();

private:
	RectangleShape cell;
	bool alive;
	STATES block_state;

};

class Grid : BgGrid{
public:
	friend class Player;

	Grid(float x, float y, float w, float h);
	void setGridActive(bool active);
	void resetGrid();
	void setAliveAll(bool v);
	void setStatesAll(Cell::STATES st);
	void setClosedArround(Vector2i pos);
	void setDestroyedArround(Vector2i pos);
	void setShowShips(bool v);
	void setName(string name);

	static bool handleClick(Grid* grid, RenderWindow* win);
	static bool handleHover(Grid* grid, RenderWindow * win);

	void draw(RenderWindow* win);

	~Grid();

private:
	bool showShips;
	Vector2f position;
	vector<Cell*> cells;
	Cell* activeCell;
	Text text_top, text_left, name;
};
