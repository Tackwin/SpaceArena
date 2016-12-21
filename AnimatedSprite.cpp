#include "AnimatedSprite.hpp"
#include "TimerManager.hpp"

AnimatedSprite::AnimatedSprite(const sf::Texture &texture, float tw, float th, float t, int n, bool loopOnce) 
	: 	sf::Sprite(texture),
		tw(tw), th(th), t(t), nFrame(n),
		loopOnce(loopOnce) {
	setTextureRect(sf::IntRect(0, 0, tw, th));
	setScale(0.66f, 0.66f);
	keyFrameChange = TimerManager::addFunction(t, "frameChange", [this](float dt)->bool{
		this->cFrame = (this->cFrame + 1) % this->nFrame;
		this->setTextureRect(sf::IntRect(this->tw * this->cFrame, 0, this->tw, this->th));
		return false;
	});
}
AnimatedSprite::~AnimatedSprite(){
	TimerManager::removeFunction(keyFrameChange);
}

void AnimatedSprite::pause(){
	TimerManager::pauseFunction(keyFrameChange);
}
void AnimatedSprite::resume(){
	TimerManager::resumeFunction(keyFrameChange);
}
void AnimatedSprite::reset(){
	cFrame = 0;
	setTextureRect(sf::IntRect(0, 0, tw, th));
}