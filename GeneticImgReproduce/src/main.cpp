#include <SFML/Graphics.hpp>
#include "Simulation/Simulation.h"

constexpr auto w = 1280;
constexpr auto h = 720;

constexpr auto file = "../img/doh.png";
constexpr auto popSize = 80;
constexpr auto rotMutChance = 0.5;
constexpr auto transMutChance = 0.5;
constexpr auto elitismM = 2;
constexpr auto threshold = 125;
constexpr auto minLineLen = 10;
constexpr auto maxLineLen = 35;

int main(int argc, char* argv[])
{
    gir::Simulation sim(w, h, popSize, transMutChance, rotMutChance, elitismM);
    sim.Prepare(file, threshold, minLineLen, maxLineLen);
    sim.Run();

    return 0;
}