#pragma once
#include <SFML/Graphics.hpp>
#include <map>

class DrawManager {
private:
	DrawManager();
	~DrawManager();

public:
	static std::vector<sf::Drawable*> drawables;

	static void draw(sf::RenderWindow &window);
};

