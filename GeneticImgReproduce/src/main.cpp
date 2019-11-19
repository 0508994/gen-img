#include <SFML/Graphics.hpp>
#include "Simulation/Simulation.h"

#define W 1280
#define H 720

#define FILE "../img/border.png"
#define POPSIZE 40
#define ROTMUTCHANCE 0.3
#define TRANSMUTCHANCE 0.4
#define ELITISM 2
#define THRESHOLD 125
#define MINLINELEN 10
#define MAXLINELEN 35

int main(int argc, char* argv[])
{
	gir::Simulation sim(W, H, POPSIZE, TRANSMUTCHANCE, ROTMUTCHANCE, ELITISM);
	sim.Prepare(FILE, THRESHOLD, MINLINELEN, MAXLINELEN);
	sim.Run();

	return 0;
}