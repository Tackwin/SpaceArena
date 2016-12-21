#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class CollisionInfo{
public:
	const sf::Image &img;

	std::vector<sf::Vector2f> collisionNorm;
	std::vector<sf::Vector2f> collisionTrans;
	sf::FloatRect globalHitBox;

	CollisionInfo() : img(sf::Image()) {};
	CollisionInfo(const sf::Image &img);
	~CollisionInfo() {};

	void transform(sf::Sprite &sprite);
	bool collideWith(const CollisionInfo &other);

	void draw(sf::RenderTarget &target);
};