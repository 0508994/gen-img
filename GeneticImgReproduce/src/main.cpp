#include <SFML/Graphics.hpp>
#include "Simulation/Simulation.h"

#define W 1280
#define H 720

int main()
{
	gir::Simulation sim(W, H);
	sim.Prepare();
	sim.Run();

	return 0;
}