#pragma once
#include <SFML/Graphics.hpp>
#include <random>
#include "Player.hpp"
#include "Bullet.hpp" 
#include "Meteor.hpp"
#include "PickUp.hpp"

#include <vector>

class World {
public:
	World();
	~World();

public:
	sf::Sprite back;

	std::vector<Bullet*> bullets;
	std::vector<Meteor*> meteors;
	std::vector<PickUp*> pickUps;
	std::vector<sf::Drawable*> drawables;
	Player* playerLocal;
	Player* otherPlayer;

	sf::View viewArena;

	std::mt19937 rng;

	int scoreLocal = 0;
	sf::Text scoreLocalText;
	int scoreOther = 0;
	sf::Text scoreOtherText;

	void update(float dt);
	void draw(sf::RenderWindow &window);

	void addExplosion(float x, float y);
};

