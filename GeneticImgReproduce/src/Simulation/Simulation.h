#pragma once
#define _USE_MATH_DEFINES
#include "Genetic/GeneticOptimizer.h"
#include "ImgOps.h"
#include "SFML/Graphics.hpp"

#include <sstream>

namespace gir {

class Simulation {
private:
    std::size_t m_WindowWidth = 0;
    std::size_t m_WindowHeight = 0;

    sf::RenderWindow m_Window;
    GeneticOptimizer m_GeneticOptimizer;

    sf::Image m_OI;
    sf::Texture m_OITexture;
    sf::Sprite m_OISprite;
    sf::RenderTexture m_CanvasTexture;
    sf::Sprite m_CanvasSprite;
    sf::VertexArray m_Va;
    sf::Clock m_Clock;
    sf::Font m_Font;
    sf::Text m_Info;

    sf::Vertex m_Background[4];
    bool m_Running = false;
    bool m_Paused = false;

    std::size_t m_ItersToRun;

public:
    Simulation(std::size_t width,
               std::size_t height,
               std::size_t popSize = 20,
               double transMutChance = 0.001,
               double rotMutChance = 0.001,
               std::size_t elitismn = 0);

public:
    void Prepare(const std::string& file, std::size_t threshold, std::size_t minLineLen, std::size_t maxLineLen);
    void Run();

private:
    void Render(const SolutionCandidate& solution);
    void HandleEvents();
};

}   // namespace gir
