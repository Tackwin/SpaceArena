#include "DrawManager.hpp"
std::vector<sf::Drawable*> DrawManager::drawables;

DrawManager::DrawManager() {
}
DrawManager::~DrawManager() {
}

void DrawManager::draw(sf::RenderWindow &window) {
	for(auto &d : drawables) {
  		window.draw(*d);
	}
}
