#pragma once 
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include "CollisionInfo.hpp"
#include "Player.hpp"

class PickUp {
public:
	CollisionInfo collision;
	sf::Sprite sprite;

	float x;
	float y;

	bool remove = false;

	std::function<void(Player&)> callBack;

	PickUp() {};
	PickUp(float x, float y, std::string texture, std::string img, std::function<void(Player&)> callback);
	~PickUp();

	void draw(sf::RenderTarget &target);
};