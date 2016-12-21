#include "CollisionInfo.hpp"
#include "Utility.hpp"

CollisionInfo::CollisionInfo(const sf::Image &img) : img(img){
	std::vector<std::pair<sf::Vector2f, int>> points;
	for(unsigned int y = 0; y < img.getSize().y; y++) {
		for(unsigned int x = 0; x < img.getSize().x; x++) {
			auto c = img.getPixel(x, y);
			if(c == sf::Color::White)
				continue;

			std::pair<sf::Vector2f, int> data({ (float)x / img.getSize().x, (float)y / img.getSize().y }, c.r << 16 | c.g << 8 | c.b);
			points.push_back(data);
		}
	}

	std::sort(std::begin(points), std::end(points), [](const std::pair<sf::Vector2f, int> &A, const std::pair<sf::Vector2f, int> &B)->bool {
		return A.second < B.second;
	});

	for(auto &p : points){
		collisionNorm.push_back(p.first);
		collisionTrans.push_back(p.first);
	}
}

bool CollisionInfo::collideWith(const CollisionInfo &other){
	if(!other.globalHitBox.intersects(globalHitBox))
		return false;

	auto A = collisionTrans;
	auto B = other.collisionTrans;
	for(int i = 0; i < A.size(); i++) {
		auto a = A[i];
		auto b = A[(i + 1) % A.size()];

		for(int j = 0; j < B.size(); j++) {
			auto c = B[j];
			auto d = B[(j + 1) % B.size()];

			if(Utility::segSegIntersect(a.x, a.y, b.x, b.y, c.x, c.y, d.x, d.y))
				return true;
		}
	}
	return false;
}

void CollisionInfo::transform(sf::Sprite &sprite){
	float c = cosf(3.1415926f * sprite.getRotation() / 180.f);
	float s = sinf(3.1415926f * sprite.getRotation() / 180.f);

	auto size = sprite.getTextureRect();
	auto o = sprite.getOrigin();
	auto p = sprite.getPosition();

	for(int i = 0; i < collisionNorm.size(); i++){
		sf::Vector2f t = collisionNorm[i];

		t.x = t.x * size.width - o.x;
		t.y = t.y * size.height - o.y;

		t = {t.x * c - t.y * s, t.x * s + t.y * c};

		t.x += p.x;
		t.y += p.y;
		
		collisionTrans[i] = t;
	}
	globalHitBox = sprite.getGlobalBounds();
}

void CollisionInfo::draw(sf::RenderTarget &target){
	for(int i = 0; i < collisionTrans.size(); i++) {
		auto P1 = collisionTrans[i];
		auto P2 = collisionTrans[(i + 1) % collisionTrans.size()];

		sf::Vertex vert[2] = {
			sf::Vertex(P1, sf::Color::Red),
			sf::Vertex(P2, sf::Color::Red)
		};
		target.draw(vert, 2, sf::Lines);
	}

	sf::Vertex vert[2] = {
		sf::Vertex({globalHitBox.left, globalHitBox.top}, sf::Color::Green),
		sf::Vertex({globalHitBox.left + globalHitBox.width, globalHitBox.top}, sf::Color::Green)
	};
	target.draw(vert, 2, sf::Lines);
	
	vert[0] = sf::Vertex({globalHitBox.left + globalHitBox.width, globalHitBox.top}, sf::Color::Green);
	vert[1] = sf::Vertex({globalHitBox.left + globalHitBox.width, globalHitBox.top + globalHitBox.height}, sf::Color::Green);
	target.draw(vert, 2, sf::Lines);
	
	vert[0] = sf::Vertex({globalHitBox.left + globalHitBox.width, globalHitBox.top + globalHitBox.height}, sf::Color::Green);
	vert[1] = sf::Vertex({globalHitBox.left, globalHitBox.top + globalHitBox.height}, sf::Color::Green);
	target.draw(vert, 2, sf::Lines);
	
	vert[0] = sf::Vertex({globalHitBox.left, globalHitBox.top + globalHitBox.height}, sf::Color::Green);
	vert[1] = sf::Vertex({globalHitBox.left, globalHitBox.top}, sf::Color::Green);
	target.draw(vert, 2, sf::Lines);
}