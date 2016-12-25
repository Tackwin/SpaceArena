#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "CollisionInfo.hpp"

class World;
class Player {
public:
	static constexpr float RAD_PER_SECOND = 3.1415926f * 1.5f;
	static constexpr float ACC = 50;

public:
	bool local;
	
	bool goLeft = false;
	bool goRight = false;
	bool goFoward = false;
	bool goBackward = false;
	
	sf::Vector2f dir = sf::Vector2f(0, 1);
	float thrust = 0.f;

	sf::Sound shootSound;
	sf::Sound hitSound;

	std::string cdShootKey;
	float cdShoot = 0.5f;
	bool canShoot = true;

	std::string cdShieldKey;
	float cdShield = 1.5f;
	bool shieldUp = true;

	std::string cdBoostKey;
	float cdBoost = 0.5f;
	float boost = 1.f;
	bool boosting = false;

	bool buffed = false;

	int structureState = 0;

	sf::Sprite shipSprite;
	sf::Sprite damageSprite;
	sf::Sprite shieldSprite;

	CollisionInfo collisionInfo;

	World *myWorld = nullptr;

public:
	Player() {}
	Player(bool local, float x, float y, World* world);
	~Player();

	void handleInput();

	void update(float dt);

	void draw(sf::RenderTarget &window);

	void shoot();
	void getHit();
	void shieldAttack();

	void heal();

	bool collideWithOAABB(sf::FloatRect &rect, sf::Vector2f pivotPoint, float r);
};

