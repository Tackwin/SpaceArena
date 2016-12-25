#include <SFML/Graphics.hpp>
#include "World.hpp"
#include "AssetsManager.hpp"
#include "InputsManager.hpp"
#include "TimerManager.hpp"
#include "Player.hpp"

//g++ -std=c++11 Main.cpp AnimatedSprite.cpp PickUp.cpp Meteor.cpp CollisionInfo.cpp AssetsManager.cpp TimerManager.cpp Bullet.cpp World.cpp Player.cpp InputsManager.cpp Utility.cpp -o ./SpaceArena -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
//i586-mingw32msvc-g++ -std=c++11 -I../SFML-MingW32/include -L../SFML-MingW32/lib Main.cpp AnimatedSprite.cpp PickUp.cpp Meteor.cpp CollisionInfo.cpp AssetsManager.cpp TimerManager.cpp Bullet.cpp World.cpp Player.cpp InputsManager.cpp Utility.cpp -o ./SpaceArena.exe -lsfml-graphics -lsfml-window -lsfml-system -O2
//i586-mingw32msvc-g++ -std=c++11 -I../SFML-MingW32/include -L../SFML-MingW32/lib Main.cpp AnimatedSprite.cpp PickUp.cpp Meteor.cpp CollisionInfo.cpp AssetsManager.cpp TimerManager.cpp Bullet.cpp World.cpp Player.cpp InputsManager.cpp Utility.cpp -o ./SpaceArena.exe -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -O2

World *world = nullptr;
void loadAssets();

int main() {
	loadAssets();
	sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!");

	world = new World();
	world->playerLocal = new Player(true, 427, 360, world);
	world->otherPlayer = new Player(false, 854, 360, world);
	world->back = sf::Sprite(AssetsManager::getTexture("backTexture"));

	TimerManager::addFunction(5.f, "fpsDisplay", [](float dt)->bool{printf("Fps: %f\n", 1.f / dt); return false;});

	sf::Clock dtClock;
	while (window.isOpen()) {
		float dt = dtClock.restart().asSeconds();
		dt = dt > 0.01666f ? 0.01666f : dt;
		InputsManager::update(window);
		TimerManager::update(dt);
		world->update(dt);

		window.clear();
		world->draw(window);
		window.display();
	}

	delete world;
	return 0;
}

void loadAssets() {
	AssetsManager::loadTexture("damage1Texture", "img/playerDamage1.png");
	AssetsManager::loadTexture("damage2Texture", "img/playerDamage2.png");
	AssetsManager::loadTexture("damage3Texture", "img/playerDamage3.png");
	AssetsManager::loadTexture("explosionTexture", "img/explosion.png");
	AssetsManager::loadTexture("meteor31Texture", "img/meteor/31.png");
	AssetsManager::loadTexture("meteor21Texture", "img/meteor/21.png");
	AssetsManager::loadTexture("meteor11Texture", "img/meteor/11.png");
	AssetsManager::loadTexture("thrustTexture", "img/thrust.png");
	AssetsManager::loadTexture("shieldTexture", "img/shield.png");
	AssetsManager::loadTexture("playerTexture", "img/player.png");
	AssetsManager::loadTexture("repairTexture", "img/repair.png");
	AssetsManager::loadTexture("laserTexture", "img/laser.png");
	AssetsManager::loadTexture("starTexture", "img/star.png");
	AssetsManager::loadTexture("buffTexture", "img/buff.png");
	AssetsManager::loadTexture("backTexture", "img/back.png");
	printf("\n");
	AssetsManager::loadImage("meteor31Collision", "img/meteor/31Collision.png");
	AssetsManager::loadImage("meteor21Collision", "img/meteor/21Collision.png");
	AssetsManager::loadImage("meteor11Collision", "img/meteor/11Collision.png");
	AssetsManager::loadImage("playerCollision", "img/playerCollision.png");
	AssetsManager::loadImage("repairCollision", "img/repairCollision.png");
	AssetsManager::loadImage("laserCollision", "img/laserCollision.png");
	AssetsManager::loadImage("buffCollision", "img/buffCollision.png");
	printf("\n");
	AssetsManager::loadSound("explosionSound", "snd/explosion.ogg");
	AssetsManager::loadSound("pickUpSound", "snd/pickUp.ogg");
	AssetsManager::loadSound("shootSound", "snd/shoot.ogg");
	AssetsManager::loadSound("hitSound", "snd/hit.ogg");
	printf("\n");
	AssetsManager::openMusic("mainMusic", "snd/music.ogg");
	printf("\n");
	AssetsManager::loadFont("font", "fnt/font.ttf");
	printf("\n");
}
