#pragma once
#include <SFML/Graphics.hpp>
#include "CollisionInfo.hpp"

class Bullet {
public:
	float r = 0.f;
	float v = 0.f;
	float life = 0.f;

	bool remove = false;

	bool local = true;

	sf::Sprite sprite;

	CollisionInfo collisionInfo;
public:

	Bullet(float r, float v, float lifeTime, const sf::Texture &texture, bool local = true);
	~Bullet();

	void draw(sf::RenderTarget &target);

	void update(float dt);

	bool toRemove() {
		return remove;
	}
};

