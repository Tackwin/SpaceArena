#include <SFML/Graphics.hpp>


class AnimatedSprite : public sf::Sprite {
public:
	float tw;
	float th;

	float t;

	int nFrame;
	int cFrame;

	std::string keyFrameChange;

	bool loopOnce = false;

	AnimatedSprite(const sf::Texture &texture, float tw, float th, float t, int n, bool loopOnce = false);
	~AnimatedSprite();

	void pause();
	void resume();
	void reset();
};