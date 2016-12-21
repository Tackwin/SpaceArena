#include "InputsManager.hpp"

bool InputsManager::keyPressed[sf::Keyboard::KeyCount];
bool InputsManager::keyJustPressed[sf::Keyboard::KeyCount];
bool InputsManager::keyJustReleased[sf::Keyboard::KeyCount];

sf::Vector2f InputsManager::mouseWorldPos;
sf::Vector2i InputsManager::mouseScreenPos;

InputsManager::InputsManager() {
}
InputsManager::~InputsManager() {
}

bool InputsManager::isKeyPressed(const sf::Keyboard::Key &key) {
	return keyPressed[key];
}
bool InputsManager::isKeyJustPressed(const sf::Keyboard::Key &key) {
	return keyJustPressed[key];
}
bool InputsManager::isKeyJustReleased(const sf::Keyboard::Key &key) {
	return keyJustReleased[key];
}

const sf::Vector2f& InputsManager::getMouseWorldPos() {
	return mouseWorldPos;
}
const sf::Vector2i& InputsManager::getMouseScreenPos() {
	return mouseScreenPos;
}

void InputsManager::update(sf::RenderWindow &window) {
	for(auto &b : keyJustPressed) {
		b = false;
	}
	for(auto &b : keyJustReleased) {
		b = false;
	}

	sf::Event event;
	while(window.pollEvent(event)) {
		if(event.type == sf::Event::Closed)
			window.close();
		if(event.type == sf::Event::KeyPressed)
			keyPressed[event.key.code] = keyJustPressed[event.key.code] = true;
		if(event.type == sf::Event::KeyReleased)
			keyPressed[event.key.code] = !(keyJustReleased[event.key.code] = true);
	}
	mouseScreenPos = sf::Mouse::getPosition(window);
	mouseWorldPos = window.mapPixelToCoords(mouseScreenPos);
}
