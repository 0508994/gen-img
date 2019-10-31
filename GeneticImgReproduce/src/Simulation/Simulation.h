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
		unsigned int m_WindowWidth;
		unsigned int m_WindowHeight;
	
		sf::RenderWindow m_Window;
		GeneticOptimizer m_GeneticOptimizer;
		
		sf::Image m_OI;
		sf::Texture m_OITexture;
		sf::Sprite m_OISprite;
		sf::RenderTexture m_CanvasTexture;

		bool m_Running;
		bool m_Paused;
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

