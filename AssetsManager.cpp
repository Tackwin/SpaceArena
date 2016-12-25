#include "AssetsManager.hpp"
#include <cassert>

std::map<std::string, sf::SoundBuffer> AssetsManager::sounds;
std::map<std::string, sf::Texture> AssetsManager::textures;
std::map<std::string, sf::Music> AssetsManager::musics;
std::map<std::string, sf::Image> AssetsManager::images;
std::map<std::string, sf::Font> AssetsManager::fonts;

AssetsManager::AssetsManager() {
}
AssetsManager::~AssetsManager() {
}

bool AssetsManager::loadTexture(const std::string &key, const std::string &path) {
	printf("Loading: %s... ", path.c_str());
	auto &ref = textures[key];

	bool loaded = ref.loadFromFile(path);
	assert(loaded && "Couldn't load file");

	printf("Succes !\n");
	return loaded;
}

const sf::Texture& AssetsManager::getTexture(const std::string &key) {
	auto it = textures.find(key);
	assert(it != std::end(textures) && "Texture don't exist");
	return it->second;
}

bool AssetsManager::loadImage(const std::string &key, const std::string &path) {
	printf("Loading: %s... ", path.c_str());
	auto &ref = images[key];

	bool loaded = ref.loadFromFile(path);
	assert(loaded && "Couldn't load file");

	printf("Succes !\n");
	return loaded;
}

const sf::Image& AssetsManager::getImage(const std::string &key) {
	auto it = images.find(key);
	assert(it != std::end(images) && "Image don't exist");
	return it->second;
}

bool AssetsManager::loadFont(const std::string &key, const std::string &path) {
	printf("Loading: %s... ", path.c_str());
	auto &ref = fonts[key];

	bool loaded = ref.loadFromFile(path);
	assert(loaded && "Couldn't load file");

	printf("Succes !\n");
	return loaded;
}

const sf::Font& AssetsManager::getFont(const std::string &key) {
	auto it = fonts.find(key);
	assert(it != std::end(fonts) && "Font don't exist");
	return it->second;
}

bool AssetsManager::loadSound(const std::string &key, const std::string &path) {
	printf("Loading: %s... ", path.c_str());
	auto &ref = sounds[key];

	bool loaded = ref.loadFromFile(path);
	assert(loaded && "Couldn't load file");

	printf("Succes !\n");
	return loaded;
}

const sf::SoundBuffer& AssetsManager::getSound(const std::string &key) {
	auto it = sounds.find(key);
	assert(it != std::end(sounds) && "Sound don't exist");
	return it->second;
}


bool AssetsManager::openMusic(const std::string &key, const std::string &path) {
	printf("Loading: %s... ", path.c_str());
	auto &ref = musics[key];

	bool loaded = ref.openFromFile(path);
	assert(loaded && "Couldn't load file");

	printf("Succes !\n");
	return loaded;
}

sf::Music& AssetsManager::getMusic(const std::string &key) {
	auto it = musics.find(key);
	assert(it != std::end(musics) && "Music don't exist");
	return it->second;
}


