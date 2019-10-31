#include "Simulation.h"

#define FPS 60

namespace gir
{
	Simulation::Simulation() {}
	
	Simulation::~Simulation() {}
	
	Simulation::Simulation(unsigned int width, unsigned int height)
		:m_WindowWidth(width),
		m_WindowHeight(height),
		m_Window(sf::VideoMode(width, height), "Image Reconstruction", sf::Style::Titlebar | sf::Style::Close),
		m_GeneticOptimizer(20, 0.001, 0.001, 2)
	{
		m_Window.setFramerateLimit(FPS);
		m_Running = true;
		m_Paused = false;
	}
	
	void Simulation::Prepare()
	{
		// Prepare the original sprite
		m_OI.loadFromFile("../img/tower_resized.jpg");
		m_OITexture.loadFromImage(m_OI);
		m_OISprite.setTexture(m_OITexture);
		m_OISprite.setScale(0.5, 0.5);

		auto oiSize = m_OI.getSize();
		m_CanvasTexture.create(oiSize.x, oiSize.y);
	
		// Prepare the genetic algorithm
		m_GeneticOptimizer.PrepareGA(m_OI, 125, 5, 15);
	}

	void Simulation::Render(const SolutionCandidate& solution)
	{
		m_Window.clear(sf::Color(125, 125, 125));
		m_Window.draw(m_OISprite);
		
		sf::CircleShape c(20.0);
		c.setFillColor(sf::Color::Blue);
		m_CanvasTexture.clear(sf::Color(150, 150, 150));
		m_CanvasTexture.draw(c);
		m_CanvasTexture.display();
		auto ts = m_CanvasTexture.getSize();
		sf::Sprite test(m_CanvasTexture.getTexture());
		test.setPosition(m_WindowWidth / 2 - ts.x / 2, m_WindowHeight / 2 - ts.y / 2);
		m_Window.draw(test);

		sf::Vertex xaxis[2] =
		{
			sf::Vertex(sf::Vector2f(0.0f, m_WindowHeight / 2), sf::Color::Black),
			sf::Vertex(sf::Vector2f(m_WindowWidth, m_WindowHeight / 2), sf::Color::Black)
		};

		sf::Vertex yaxis[2] =
		{
			sf::Vertex(sf::Vector2f(m_WindowWidth / 2, 0.0f), sf::Color::Black),
			sf::Vertex(sf::Vector2f(m_WindowWidth / 2, m_WindowHeight), sf::Color::Black)
		};
		
		m_Window.draw(xaxis, 2, sf::Lines);
		m_Window.draw(yaxis, 2, sf::Lines);

		m_Window.display();
	}

	void Simulation::Run()
	{
		while (m_Running)
		{
			HandleEvents();
			if (!m_Paused)
			{
				auto solution = m_GeneticOptimizer.RunIteration();
				Render(solution);
				m_Paused = true;
			}
		}

		m_Window.close();
	}

	void Simulation::HandleEvents()
	{
		sf::Event e;
		while (m_Window.pollEvent(e))
		{
			// All for now
			if (e.type == sf::Event::Closed)
				m_Running = false;
		}

	}
}
