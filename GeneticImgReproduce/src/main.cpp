#include <SFML/Graphics.hpp>
#include "Simulation/Simulation.h"

#define W 1280
#define H 720

#define FILE "../img/tower_resized.jpg"
#define THRESHOLD 125
#define MINLINELEN 5
#define MAXLINELEN 33

int main(int argc, char* argv[])
{
	gir::Simulation sim(W, H);
	sim.Prepare(FILE, THRESHOLD, MINLINELEN, MAXLINELEN);
	sim.Run();

	return 0;
}