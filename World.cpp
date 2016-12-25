#include "World.hpp"
#include "Utility.hpp"
#include "TimerManager.hpp"
#include "AssetsManager.hpp"
#include <string>

World::World() :
	scoreOtherText("", AssetsManager::getFont("font")),
	scoreLocalText("", AssetsManager::getFont("font")){

	AssetsManager::getMusic("mainMusic").setLoop(true);
	AssetsManager::getMusic("mainMusic").setVolume(15);
	AssetsManager::getMusic("mainMusic").play();

	pickUpSound.setBuffer(AssetsManager::getSound("pickUpSound"));
	explosionSound.setBuffer(AssetsManager::getSound("explosionSound"));

	std::uniform_real_distribution<float> posRng(-3000, 3000);
	std::uniform_real_distribution<float> oneRng(0, 1);
	std::uniform_real_distribution<float> radRng(-3.1415926, 3.1415926);
	std::uniform_real_distribution<float> vRng(10, 20);
	std::uniform_real_distribution<float> aRng(10, 20);
	for(int n = 0; n < 150; n++){
		meteors.push_back(new Meteor(3, oneRng(rng) * 10000 - 5000,
									 oneRng(rng) * 10000 - 5000, radRng(rng), vRng(rng), aRng(rng), this));
	}
	for(int n = 0; n < 200; n++){
		pickUps.push_back(new PickUp(oneRng(rng) * 10000 - 5000, oneRng(rng) * 10000 - 5000,
						  std::string("repairTexture"), std::string("repairCollision"),
						  std::function<void(Player&)>([](Player& P)->void{
						  	P.heal();
						  })));
	}
	for(int n = 0; n < 200; n++){
		pickUps.push_back(new PickUp(oneRng(rng) * 10000 - 5000, oneRng(rng) * 10000 - 5000,
						  std::string("buffTexture"), std::string("buffCollision"),
						  std::function<void(Player&)>([](Player& P)->void{
						  	P.buffed = true;
						  	TimerManager::addFunctionOnce(2.f, "buffPlayer", [&P](float)->bool{
						  		P.buffed = false;
						  		return true;
						  	});
						  })));
	}

	for(int n = 0; n < 2000; n++){
		auto *A = new AnimatedSprite(AssetsManager::getTexture("starTexture"), 10, 10, 0.1f, 5);
		A->setPosition(oneRng(rng) * 10000 - 5000, oneRng(rng) * 10000 - 5000);
		A->setScale(1.3f, 1.3f);
		A->pause();
		TimerManager::addFunctionOnce(oneRng(rng) * 2.f, "starOffset", [A](float)->bool{
			A->resume();
			return true;
		});
		drawables.push_back(A);	
	}
}

World::~World() {
	AssetsManager::getMusic("mainMusic").stop();
	if(pickUpSound.getStatus() == sf::SoundSource::Playing)
		pickUpSound.stop();
	if(explosionSound.getStatus() == sf::SoundSource::Playing)
		explosionSound.stop();

	delete playerLocal;
	delete otherPlayer;
	for(auto &d : drawables){
		delete d;
	}
	for(auto &b : bullets) {
		delete b;
	}
	for(auto &m : meteors) {
		delete m;
	}
	for(auto &p : pickUps) {
		delete p;
	}
	for(auto &p : teamA) {
		delete p;
	} for (auto &p : teamB) {
		delete p;
	}
}

void World::update(float dt) {
	playerLocal->handleInput();
	otherPlayer->handleInput();
	playerLocal->update(dt);
	otherPlayer->update(dt);

	for(auto &b : bullets)
		b->update(dt);
	for(auto &m : meteors)
		m->update(dt);

	for(auto &p : pickUps){
		if(p->collision.collideWith(playerLocal->collisionInfo)){
			p->remove = true;
			p->callBack(*playerLocal);
			scoreLocal++;

			if(pickUpSound.getStatus() == sf::SoundSource::Playing)
				pickUpSound.stop();
			pickUpSound.play();
		} else if(p->collision.collideWith(otherPlayer->collisionInfo)){
			p->remove = true;
			p->callBack(*otherPlayer);
			scoreOther++;

			if(pickUpSound.getStatus() == sf::SoundSource::Playing)
				pickUpSound.stop();
			pickUpSound.play();
		}
	}
	for(auto &b : bullets) {
		if(b->local && b->collisionInfo.collideWith(otherPlayer->collisionInfo)) {
			b->remove = true;
			addExplosion(b->sprite.getPosition().x, b->sprite.getPosition().y);
			otherPlayer->getHit();
			scoreLocal++;
		} else if(!b->local && b->collisionInfo.collideWith(playerLocal->collisionInfo)) {
			b->remove = true;
			addExplosion(b->sprite.getPosition().x, b->sprite.getPosition().y);
			playerLocal->getHit();
			scoreOther++;
		} else {
			bool breakFlag = false;
			for(auto &m : meteors){
				if(m->collision.collideWith(b->collisionInfo)){
					m->explode();
					addExplosion(b->sprite.getPosition().x, b->sprite.getPosition().y);
					breakFlag = true;
					if(b->local)
						scoreLocal++;
					else
						scoreOther++;
					break;
				}
			}
			if(breakFlag){
				b->remove = true;
				break;
			}
		}
	}
	for(auto &m : meteors){
		if(m->collision.collideWith(playerLocal->collisionInfo)){
			playerLocal->getHit();
			m->explode();
		} if (m->collision.collideWith(otherPlayer->collisionInfo)){
			otherPlayer->getHit();
			m->explode();
		}
	}

	for(int i = 0; i < bullets.size(); i++) {
		if(!bullets[i]->toRemove())
			continue;
		delete bullets[i];
		bullets.erase(std::begin(bullets) + i);
		i--;
	}
	for(int i = 0; i < meteors.size(); i++) {
		if(!meteors[i]->toRemove)
			continue;
		delete meteors[i];
		meteors.erase(std::begin(meteors) + i);
		i--;
	}
	for(int i = 0; i < pickUps.size(); i++) {
		if(!pickUps[i]->remove)
			continue;
		delete pickUps[i];
		pickUps.erase(std::begin(pickUps) + i);
		i--;
	}

	sf::Vector2f pToP;
	pToP.x = abs(playerLocal->shipSprite.getPosition().x - otherPlayer->shipSprite.getPosition().x);
	pToP.y = abs(playerLocal->shipSprite.getPosition().y - otherPlayer->shipSprite.getPosition().y);

	pToP.x = pToP.x > 1000 ? pToP.x < 3000 ? pToP.x : 3000 : 1000;
	pToP.y = pToP.y > 562 ? pToP.y < 1687 ? pToP.y : 1687 : 562;

	if(pToP.x > pToP.y * (1280.f / 720.f)) {
		pToP.x += 400;
		pToP.y = pToP.x / 1.77777778f;
	} else {
		pToP.y += 224;
		pToP.x = pToP.y * 1.77777778f;
	}

	viewArena.setCenter(otherPlayer->shipSprite.getPosition().x + (playerLocal->shipSprite.getPosition().x - otherPlayer->shipSprite.getPosition().x) / 2.f,
						otherPlayer->shipSprite.getPosition().y + (playerLocal->shipSprite.getPosition().y - otherPlayer->shipSprite.getPosition().y) / 2.f);
	viewArena.setSize(pToP);

	scoreLocalText.setString(std::string("Player 1: ") + std::to_string(scoreLocal));
	scoreOtherText.setString(std::string("Player 2: ") + std::to_string(scoreOther));

	scoreLocalText.setPosition(10, 10);
	scoreOtherText.setPosition(1270 - scoreOtherText.getGlobalBounds().width, 10);
}

void World::draw(sf::RenderWindow &window) {
	window.setView(viewArena);

	auto viewSize = viewArena.getSize();
	auto p = viewArena.getCenter();
	p.x -= viewSize.x;
	p.y -= viewSize.y;
	p.x = (int)p.x / 256;
	p.y = (int)p.y / 256;
	for(int y = -1; y < 1 + 2 * viewSize.y / 256; y++) {
		for(int x = -1; x < 1 + 2 * viewSize.x / 256; x++) {
			back.setPosition((p.x + x) * 256, (p.y + y) * 256);
			window.draw(back);
		}
	}

	for(auto &d : drawables){
		window.draw(*d);
	}
	for(auto &m : meteors) {
		m->draw(window);
	}
	for(auto &b : bullets) {
		b->draw(window);
	}
	for(auto &p : pickUps) {
		p->draw(window);
	}
	otherPlayer->draw(window);
	playerLocal->draw(window);

	window.setView(window.getDefaultView());

	window.draw(scoreOtherText);
	window.draw(scoreLocalText);
}


void World::addExplosion(float x, float y){
	if(explosionSound.getStatus() == sf::SoundSource::Playing)
		explosionSound.stop();
	explosionSound.play();

	AnimatedSprite* A = new AnimatedSprite(AssetsManager::getTexture("explosionTexture"), 189, 189, 0.025f, 9);
	A->setOrigin(A->getTextureRect().width / 2.f, A->getTextureRect().height / 2.f);
	A->setPosition(x, y);
	drawables.push_back(A);
	TimerManager::addFunctionOnce(0.25f, "null", [&, A](float)->bool{
		auto it = std::find(drawables.begin(), drawables.end(), A);
		if(it == drawables.end())
			return false;

		delete A;
		drawables.erase(it);
		return false;
	});
}
