#include "Meteor.hpp"
#include "World.hpp"
#include "AssetsManager.hpp"
#include <string>

Meteor::Meteor(int stage, float x, float y, float r, float v, float a, World *world): 
	sprite(AssetsManager::getTexture(std::string("meteor") + std::to_string(stage) + "1Texture")),
	r(r), v(v), a(a), stage(stage),
	myWorld(world),
	collision(AssetsManager::getImage(std::string("meteor") + std::to_string(stage) + "1Collision")){
	sprite.setOrigin(sprite.getTextureRect().width / 2.f, sprite.getTextureRect().width / 2.f);
	sprite.setPosition(x, y);
}

void Meteor::update(float dt){
	sprite.move(cosf(r) * v * dt, sinf(r) * v * dt);
	sprite.setRotation(sprite.getRotation() + a * dt);
	collision.transform(sprite);
}

void Meteor::draw(sf::RenderTarget &target){
	target.draw(sprite);
	collision.draw(target);
}

void Meteor::explode(){
	if(toRemove)
		return;
	toRemove = true;
	if(stage <= 1)
		return;

	std::uniform_real_distribution<float> rRng(-3.1415926, 3.1415926);
	myWorld->meteors.push_back(new Meteor(stage - 1, sprite.getPosition().x, sprite.getPosition().y, r + rRng(myWorld->rng), v*2, a*2, myWorld));
	myWorld->meteors.push_back(new Meteor(stage - 1, sprite.getPosition().x, sprite.getPosition().y, r + rRng(myWorld->rng), v*2, a*2, myWorld));
} 