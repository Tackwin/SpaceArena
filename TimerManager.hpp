#pragma once
#include <map>
#include <functional>

struct Function {
	float time = 0;
	float timer = 0;
	bool running = true;
	bool once = false;

	bool toRemove = false;
	std::function<bool(float)> fun;

	Function() {}
	Function(float time, float timer, bool running, bool once, const std::function<bool(float)> &fun) : time(time), timer(timer), running(running), once(once), fun(fun) {};
};

class TimerManager {
private:
	static std::map<std::string, Function> functions;
private:
	TimerManager();
	~TimerManager();

public:
	static std::string addFunctionOnce(float timer, const std::string &key, const std::function<bool(float)> &f);
	static std::string addFunction(float timer, const std::string &key, const std::function<bool(float)> &f);
	static void resetTimerFunction(const std::string &key);
	static void restartFunction(const std::string &key);
	static void removeFunction(const std::string &key);
	static void resumeFunction(const std::string &key);
	static void pauseFunction(const std::string &key);
	static void callFunction(const std::string &key);

	static void update(float dt);
};

