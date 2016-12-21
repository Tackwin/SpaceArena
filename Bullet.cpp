#include "Bullet.hpp"
#include "Utility.hpp"
#include "AssetsManager.hpp"


Bullet::Bullet(float r, float v, float lifeTime, const sf::Texture &texture, bool local) : 
	sprite(texture), 
	life(lifeTime), 
	r(r), 
	v(v), 
	local(local),
	collisionInfo(AssetsManager::getImage("laserCollision")) {

	auto rect = sprite.getTextureRect();
	sprite.setOrigin(rect.width / 2.f, rect.height);
	sprite.setRotation(180 * r / 3.1415926f);
}

Bullet::~Bullet() {
}

void Bullet::update(float dt) {
	sprite.move(cosf(r) * v * dt, sinf(r) * v * dt);
	life -= dt;
	remove = life <= 0;
	collisionInfo.transform(sprite);
}

void Bullet::draw(sf::RenderTarget &target) {
	target.draw(sprite);
	collisionInfo.draw(target);
}

