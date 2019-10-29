#pragma once
#include <SFML/Graphics.hpp>
#include "Genetic/GeneticOptimizer.h"
#include "ImgOps.h"

namespace gir
{
	class Simulation
	{
	private:
		bool m_Running = false;
		bool m_Redraw = true;
		unsigned int m_WindowWidth;
		unsigned int m_WindowHeight;
		sf::RenderWindow m_Window;

		GeneticOptimizer m_GeneticOptimizer;
	public:
		Simulation();
		Simulation(unsigned int width, unsigned int height);
		~Simulation();

		void Render();
		void Run();
		void HandleEvents();
	};
}

