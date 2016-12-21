#include "PickUp.hpp"
#include "AssetsManager.hpp"

PickUp::PickUp(float x, float y, std::string texture, std::string img, std::function<void(Player&)> callback) 
	: sprite(AssetsManager::getTexture(texture)),
	  collision(AssetsManager::getImage(img)),
	  callBack(callback) {
	sprite.setPosition(x, y);
	collision.transform(sprite);
}
PickUp::~PickUp(){

}

void PickUp::draw(sf::RenderTarget &target){
	target.draw(sprite);
	collision.draw(target);
}