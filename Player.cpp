#include "Player.hpp"
#include "World.hpp"
#include "AssetsManager.hpp"
#include "InputsManager.hpp"
#include "TimerManager.hpp"
#include "Utility.hpp"
#include "Bullet.hpp"

#include <cassert>

Player::Player(bool local, float x, float y, World* world) : 
	shipSprite(AssetsManager::getTexture("playerTexture")), 
	myWorld(world), 
	local(local),
	collisionInfo(AssetsManager::getImage("playerCollision")) {

	shootSound.setBuffer(AssetsManager::getSound("shootSound"));
	hitSound.setBuffer(AssetsManager::getSound("hitSound"));

	shipSprite.setOrigin(25.f, 50.f);
	shipSprite.setPosition(x, y);
	
	cdShootKey = TimerManager::addFunction(cdShoot, "playerShootCd", [&](float)->bool {
		canShoot = true;
		TimerManager::pauseFunction(cdShootKey);
		return false;
	});
	TimerManager::pauseFunction(cdShootKey);

	cdShieldKey = TimerManager::addFunction(cdShield, "playerShieldCd", [&](float)->bool {
		shieldUp = true;
		TimerManager::pauseFunction(cdShieldKey);
		return false;
	});
	TimerManager::pauseFunction(cdShieldKey);

	cdBoostKey = TimerManager::addFunction(cdBoost, "playerBoostCd", [&](float dt) -> bool{
		boost += dt;
		return false;
	});
	TimerManager::pauseFunction(cdBoostKey);
}

Player::~Player() {
	if(shootSound.getStatus() == sf::SoundSource::Playing)
		shootSound.stop();
	if(hitSound.getStatus() == sf::SoundSource::Playing)
		hitSound.stop();
}

void Player::handleInput() {
	sf::Keyboard::Key keyLeft, keyRight, keyFoward, keyBackward, keyShoot, keyBoost;
	if(local) {
		keyLeft = sf::Keyboard::Q;
		keyRight = sf::Keyboard::D;
		keyFoward = sf::Keyboard::Z;
		keyBackward = sf::Keyboard::S;
		keyShoot = sf::Keyboard::Space;
		keyBoost = sf::Keyboard::LShift;
	} else {
		keyLeft = sf::Keyboard::J;
		keyRight = sf::Keyboard::L;
		keyFoward = sf::Keyboard::I;
		keyBackward = sf::Keyboard::K;
		keyShoot = sf::Keyboard::Return;
		keyBoost = sf::Keyboard::RShift;
	}

	goLeft = goRight = goFoward = goBackward = false;
	if(InputsManager::isKeyPressed(keyLeft)) {
		goLeft = true;
	} else if(InputsManager::isKeyPressed(keyRight)) {
		goRight = true;
	} if(InputsManager::isKeyPressed(keyFoward)) {
		goFoward = true;
	} else if(InputsManager::isKeyPressed(keyBackward)) {
		goBackward = true;
	}
	if(InputsManager::isKeyJustPressed(keyShoot) && canShoot) {
		shoot();
		canShoot = false;
		TimerManager::resumeFunction(cdShootKey);
	}
	if(InputsManager::isKeyJustPressed(keyBoost)) {
		TimerManager::pauseFunction(cdBoostKey);
		boosting = true;
	} else if(InputsManager::isKeyJustReleased(keyBoost)) {
		TimerManager::resumeFunction(cdBoostKey);
		TimerManager::restartFunction(cdBoostKey);
		boosting = false;
	}
}

float effThrust(float x) {
	return 0.5f + (1 + expf(-8*x -4) - 0.5 * (1 + expf(-8*x + 3.2))) / ((1 + expf(-8*x + 3.2f)) * (1 + expf(-8*x - 4)));
}

void Player::update(float dt) {
	float rdir = atan2f(dir.y, dir.x);
	if(goLeft) {
		rdir -= RAD_PER_SECOND * dt;
	} else if(goRight) {
		rdir += RAD_PER_SECOND * dt;
	}
	dir.x = cosf(rdir);
	dir.y = sinf(rdir);

	float boostThrust = 1.f;
	if(boosting && boost > 0) {
		boost -= dt;
		boostThrust = 1.5f;
	}

	if(goFoward) {
		thrust += ACC * dt * boostThrust;
	} if(goBackward) {
		thrust -= ACC * dt * boostThrust;
	} 
	thrust *= 1 - 3.f * dt;

	float effectiveThrust = effThrust(thrust);
	shipSprite.setPosition(shipSprite.getPosition().x + dir.x * thrust * effectiveThrust, shipSprite.getPosition().y + dir.y * thrust * effectiveThrust);
	shipSprite.setRotation(180 * rdir / 3.1415926f);

	damageSprite.setPosition(shipSprite.getPosition());
	damageSprite.setRotation(shipSprite.getRotation());

	collisionInfo.transform(shipSprite);
}

void Player::draw(sf::RenderTarget &window) {
	window.draw(shipSprite);
	window.draw(damageSprite);
	collisionInfo.draw(window);
}

void Player::shoot() {
	if(shootSound.getStatus() == sf::SoundSource::Playing)
		shootSound.stop();
	shootSound.play();

	auto b = new Bullet(atan2f(dir.y, dir.x), 1500, 1.f, AssetsManager::getTexture("laserTexture"), local);
	b->sprite.setPosition(shipSprite.getPosition());
	myWorld->bullets.push_back(b);
	if(!buffed)
		return;

	for(int i = 0; i < 4; i++){
		float a = atan2f(dir.y, dir.x);
		a += ((i + (i > 1 ? 1 : 0)) * (3.1415926f / 4.f) / 5.f) - 3.1415926f / 8.f + 3.1415926f / 40.f;
		b = new Bullet(a, 1500, 1.f, AssetsManager::getTexture("laserTexture"), local);
		b->sprite.setPosition(shipSprite.getPosition());
		myWorld->bullets.push_back(b);
	}
}

void Player::getHit() {
	if(hitSound.getStatus() == sf::SoundSource::Playing)
		hitSound.stop();
	hitSound.play();

	structureState++;
	assert(structureState <= 3 && "GameOver");

	std::string textureKey = std::string("damage") + std::to_string(structureState) + std::string("Texture");
	damageSprite.setColor(sf::Color::White);
	damageSprite.setTexture(AssetsManager::getTexture(textureKey));
	damageSprite.setOrigin(shipSprite.getOrigin());
	damageSprite.setPosition(shipSprite.getPosition());
	damageSprite.setRotation(shipSprite.getRotation());

	//blinking
	std::string blinkUpKey = TimerManager::addFunction(0.2f, "blinkUp", [&](float) -> bool {
		damageSprite.setColor(sf::Color::White);
		return false;
	});
	std::string blinkDown = TimerManager::addFunction(0.2f, "blinkDown", [&](float) -> bool {
		damageSprite.setColor(sf::Color::Transparent);
		return false;
	});
	TimerManager::pauseFunction(blinkDown);

	TimerManager::addFunctionOnce(0.1f, "blinkStartDown", [blinkDown](float) -> bool {
		TimerManager::resumeFunction(blinkDown);
		TimerManager::callFunction(blinkDown);
		return true;
	});
	TimerManager::addFunctionOnce(0.6f, "stopBlinking", [&, blinkDown, blinkUpKey](float)->bool {
		TimerManager::removeFunction(blinkDown);
		TimerManager::removeFunction(blinkUpKey);
		damageSprite.setColor(sf::Color::White);
		return true;
	});
}
void Player::shieldAttack() {
	shieldUp = false;
	TimerManager::resumeFunction(cdShieldKey);

}

void Player::heal(){
	if(structureState <= 0)
		return;
	if(structureState == 1){
		damageSprite.setTexture(sf::Texture());
		damageSprite.setColor(sf::Color::Transparent);
		structureState--;
		return;
	}
	damageSprite.setColor(sf::Color::White);
	damageSprite.setTexture(AssetsManager::getTexture(std::string("damage") + std::to_string(--structureState) + std::string("Texture")));
	damageSprite.setOrigin(shipSprite.getOrigin());
	damageSprite.setPosition(shipSprite.getPosition());
	damageSprite.setRotation(shipSprite.getRotation());

}