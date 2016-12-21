#pragma once
#include <SFML/Graphics.hpp>

class InputsManager {
private:
	InputsManager();
	~InputsManager();

private:
	static bool keyPressed[sf::Keyboard::KeyCount];
	static bool keyJustPressed[sf::Keyboard::KeyCount];
	static bool keyJustReleased[sf::Keyboard::KeyCount];

	static sf::Vector2f mouseWorldPos;
	static sf::Vector2i mouseScreenPos;

public:
	static void update(sf::RenderWindow &window);

	static bool isKeyPressed(const sf::Keyboard::Key &key);
	static bool isKeyJustPressed(const sf::Keyboard::Key &key);
	static bool isKeyJustReleased(const sf::Keyboard::Key &key);

	static const sf::Vector2f& getMouseWorldPos();
	static const sf::Vector2i& getMouseScreenPos();
};

