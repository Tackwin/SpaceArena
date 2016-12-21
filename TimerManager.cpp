#include "TimerManager.hpp"
#include <cassert>
#include <string>

std::map<std::string, Function> TimerManager::functions;

TimerManager::TimerManager() {
}

TimerManager::~TimerManager() {
}

std::string TimerManager::addFunction(float timer, const std::string &key, const std::function<bool(float)> &f) {
	int n = 0;
	for(auto &d : functions) {
		if(d.first.find(key) != std::string::npos)
			n++;
	}
	functions[key + std::to_string(n)] = Function(timer, timer, true, false, f);
	return key + std::to_string(n);
}
std::string TimerManager::addFunctionOnce(float timer, const std::string &key, const std::function<bool(float)> &f) {
	int n = 0;
	for(auto &d : functions) {
		if(d.first.find(key) != std::string::npos)
			n++;
	}
	functions[key + std::to_string(n)] = Function(timer, timer, true, true, f);
	return key + std::to_string(n);
}
void TimerManager::resetTimerFunction(const std::string &key) {
	auto it = functions.find(key);
	assert(it != std::end(functions) && "Can't find the function");
	it->second.time = it->second.timer;
}
void TimerManager::restartFunction(const std::string &key) {
	auto it = functions.find(key);
	assert(it != std::end(functions) && "Can't find the function");
	it->second.timer = it->second.time;
}
void TimerManager::callFunction(const std::string &key) {
	auto it = functions.find(key);
	assert(it != std::end(functions) && "Can't find the function");
	it->second.fun(0);
}
void TimerManager::removeFunction(const std::string &key) {
	auto it = functions.find(key);
	assert(it != std::end(functions) && "Can't find the function");
	it->second.toRemove = true;
}
void TimerManager::pauseFunction(const std::string &key) {
	auto it = functions.find(key);
	assert(it != std::end(functions) && "Can't find the function");
	it->second.running = false;
}
void TimerManager::resumeFunction(const std::string &key) {
	auto it = functions.find(key);
	assert(it != std::end(functions) && "Can't find the function");
	it->second.running = true;
}

void TimerManager::update(float dt) {
	size_t prevSize = functions.size();
	for(auto it = std::begin(functions); it != std::end(functions);){
		auto &f = it->second;
		if(f.toRemove) {
			functions.erase(it++);
			continue;
		}
		if(!f.running) {
			++it;
			continue;
		}
		f.timer -= dt;
		if(f.timer <= 0) {
			f.timer = f.time;
			f.toRemove = f.fun(dt);
			if(f.once) {
				functions.erase(it++);
				continue;
			}
		}
		++it;
	}
}
