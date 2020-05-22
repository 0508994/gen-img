#pragma once
#define _USE_MATH_DEFINES
#include <sstream>
#include "SFML/Graphics.hpp"
#include "Genetic/GeneticOptimizer.h"
#include "ImgOps.h"

namespace gir
{
	class Simulation
	{
	private:
		unsigned int m_WindowWidth = 0;
		unsigned int m_WindowHeight = 0;
	
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

		unsigned int m_ItersToRun;
	public:
		Simulation(unsigned int width, unsigned int height, unsigned int popSize = 20,
				   double transMutChance = 0.001, double rotMutChance = 0.001, unsigned int elitismn = 0);

		void Prepare(std::string file, unsigned int threshold, unsigned int minLineLen, unsigned int maxLineLen);
		void Render(const SolutionCandidate& solution);
		void Run();
		void HandleEvents();
	};
}

