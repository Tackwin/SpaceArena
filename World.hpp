#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <random>
#include "Player.hpp"
#include "Bullet.hpp" 
#include "Meteor.hpp"
#include "PickUp.hpp"
#include "AnimatedSprite.hpp"

#include <vector>

class World {
public:
	World();
	~World();

public:
	sf::Sprite back;

	sf::Sound pickUpSound;
	sf::Sound explosionSound;

	std::vector<sf::Drawable*> drawables;
	std::vector<Bullet*> bullets;
	std::vector<Meteor*> meteors;
	std::vector<PickUp*> pickUps;
	std::vector<Player*> teamA;
	std::vector<Player*> teamB;
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

	void listenConnection();
	void addExplosion(float x, float y);
};

