#pragma once
#include <SFML/Graphics.hpp>
#include "CollisionInfo.hpp"

class World;
class Meteor {
public:

	float r = 0;
	float v = 0;
	float a = 0;
	int stage = 0;

	sf::Sprite sprite;
	CollisionInfo collision;

	World *myWorld;

	bool toRemove = false;

	Meteor(int stage, float x, float y, float r, float v, float a, World *world);
	~Meteor(){};

	void update(float dt);
	void draw(sf::RenderTarget &target);

	void explode();
};