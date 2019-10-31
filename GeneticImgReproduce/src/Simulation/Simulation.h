#pragma once
#define _USE_MATH_DEFINES
#include <SFML/Graphics.hpp>
#include "Genetic/GeneticOptimizer.h"
#include "ImgOps.h"

namespace gir
{
	class Simulation
	{
	private:
		bool m_Running = false;
		bool m_Paused = true;
		unsigned int m_WindowWidth;
		unsigned int m_WindowHeight;
		
		sf::RenderWindow m_Window;
		sf::Image m_OI;
		sf::Texture m_OITexture;
		sf::Sprite m_OISprite;
		sf::RenderTexture m_CanvasTexture;

		GeneticOptimizer m_GeneticOptimizer;
	public:
		Simulation();
		Simulation(unsigned int width, unsigned int height);
		~Simulation();

		void Prepare();
		void Render(const SolutionCandidate& solution);
		void Run();
		void HandleEvents();
	};
}

