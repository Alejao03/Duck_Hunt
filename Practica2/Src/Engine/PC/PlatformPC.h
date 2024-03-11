#include <cstdint>
#include <list>
#include <string>
#include "InputListener.h"

union SDL_Event;
class SDLEventListener;

class PlatformPC
{
private:
	PlatformPC();
	~PlatformPC();
	static PlatformPC* instance_;
	bool privateInit();

	// Lista de input listeners. Esto permite que varios objetos puedan escuchar eventos de teclado (en este caso eventos SDL en general)
// Si usaramos una cola donde guardar los eventos a la que cualquiera pueda acceder seria imposible que más de 1 objeto la consuma
	std::list<SDLEventListener*> listeners;
	void send(SDL_Event sdlEvent) const;
public:
	static bool Init();
	static void Release();
	static PlatformPC* Instance();
	uint32_t getTimeSinceStart() const;
	void  delay(uint32_t miliseconds) const;
	float randomFloat(float min, float max) const;
	int randomInt(int min, int max) const;//[min, max]
	void addInputListener(SDLEventListener* inputListener);
	void removeInputListener(SDLEventListener* inputListener);

	bool tick() const;

	// Dar soporte a la apertura de ficheros (FILE*).
	FILE* openBinaryFile(const std::string& name) const;
};
