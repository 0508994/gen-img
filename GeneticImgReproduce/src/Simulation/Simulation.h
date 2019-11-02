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
		unsigned int m_WindowWidth;
		unsigned int m_WindowHeight;
	
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
		sf::Text m_FpsText;

		bool m_Running;
		bool m_Paused;
		double m_Test = 0;
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

