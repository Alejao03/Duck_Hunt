#pragma once
#include <cstdint>
#include <thread>
#include "Command.h"

//#include <queue>
#include "readerwriterqueue.h"
#include <mutex>
#include <condition_variable>

class RenderThread
{
private:
	std::thread hilo_;
	moodycamel::ReaderWriterQueue<Command> commands_;
	volatile int frames_Left_ = 0;
	volatile bool acabar_ = false;
	std::mutex mutex_;
	std::condition_variable condition_variable;

	void main();

public:
	void start(); //lanza hilo
	void enqueCommand(Command comando);
	void stop();
	int getFramesLeft() const;
};

