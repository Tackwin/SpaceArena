#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
class Utility {
public:
	static std::vector<sf::Vector2f> getHitBoxInfo(const std::string &path) {
		sf::Image img;
		img.loadFromFile(path);

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

		std::vector<sf::Vector2f> r;
		for(auto &p : points)
			r.push_back(p.first);
		return r;
	}

	static bool segSegIntersect(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
		float s1x, s1y, s2x, s2y;
		s1x = x2 - x1;
		s1y = y2 - y1;
		s2x = x4 - x3;
		s2y = y4 - y3;
		float d = (-s2x * s1y + s1x * s2y);
		if(d == 0)
			return false;
		float s = (-s1y * (x1 - x3) + s1x * (y1 - y3)) / d;
		float t = (s2x * (y1 - y3) - s2y * (x1 - x3)) / d;
		return s >= 0 && s <= 1 && t >= 0 && t <= 1;
	}

	static bool hitBoxHitBox(std::vector<sf::Vector2f> &A, std::vector<sf::Vector2f> &B) {
		for(int i = 0; i < A.size(); i++) {
			auto a = A[i];
			auto b = A[(i + 1) % A.size()];

			for(int j = 0; j < B.size(); j++) {
				auto c = B[j];
				auto d = B[(j + 1) % B.size()];

				if(segSegIntersect(a.x, a.y, b.x, b.y, c.x, c.y, d.x, d.y))
					return true;
			}
		}
		return false;
	}
};