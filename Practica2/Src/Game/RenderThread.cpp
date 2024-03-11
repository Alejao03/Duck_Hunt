#include "RenderThread.h"
#include <iostream>
#include "Renderer.h"
#include <mutex>
#include <condition_variable>

void RenderThread::start() {
	hilo_ = std::thread(&RenderThread::main, this); 
	//dado que main es un miembro de la clase necesitas pasarle una instancia de la misma donde ejecutar el hilo
}

void RenderThread::main() {
	// Lock used in the condition_variable to not make an active wait
	std::unique_lock<std::mutex> lock(mutex_);

	while (!acabar_) {
		condition_variable.wait(lock, [this] { return !(commands_.size_approx() <= 0) || acabar_; });

		Command comando = {};
		if (commands_.try_dequeue(comando)) {
			switch (comando.type) {
			case CLEAR:
				Renderer::Instance()->clear(comando.color);
				break;
			case DRAW_SPRITE: {
				const Sprite* sprite = comando.spriteParam.sprite;
				const int posX = comando.spriteParam.x;
				const int posY = comando.spriteParam.y;
				const int width = comando.spriteParam.width;
				const int height = comando.spriteParam.height;
				
				if (sprite != nullptr)
					sprite->draw(posX, posY, width, height);
			}				
				break;
			case PRESENT:
				frames_Left_--;
				Renderer::Instance()->present();
				break;
			case STOP:
				//acabar_ = true;
				break;
			default:
				std::cout << "UNKNOWN COMMAND in RenderThread::main()" << std::endl;
				break;
			}
		}		
	}
}

void RenderThread::enqueCommand(Command comando) {
	if(comando.type == PRESENT)
		frames_Left_++;

	commands_.emplace(comando);
	condition_variable.notify_one(); // Notify the thread that there is a new command
}

void RenderThread::stop() {
	//mandar mensaje----
	Command comando = {};
	comando.type = STOP;
	comando.color = 10;
	acabar_ = true;
	enqueCommand(comando);
	hilo_.join();
}

int RenderThread::getFramesLeft() const
{
	return frames_Left_;
}
