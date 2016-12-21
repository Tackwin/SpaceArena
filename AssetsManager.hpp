#pragma once
#include <SFML/Graphics.hpp>
#include <map>

class AssetsManager {
private:
	AssetsManager();
	~AssetsManager();

public:
	static bool loadTexture(const std::string &key, const std::string &path);
	static const sf::Texture& getTexture(const std::string &key);

	static bool loadImage(const std::string &key, const std::string &path);
	static const sf::Image& getImage(const std::string &key);

	static bool loadFont(const std::string &key, const std::string &path);
	static const sf::Font& getFont(const std::string &key);

private:
	static std::map<std::string, sf::Texture> textures;
	static std::map<std::string, sf::Image> images;
	static std::map<std::string, sf::Font> fonts;
};

