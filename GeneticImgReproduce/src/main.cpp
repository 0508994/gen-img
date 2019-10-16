#include <SFML/Graphics.hpp>
#include "Simulation/Simulation.h"

#define W 800
#define H 600

int main()
{
	gir::Simulation sim(W, H);
	sim.Run();

	return 0;
}