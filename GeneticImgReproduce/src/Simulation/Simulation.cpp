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

		m_Font.loadFromFile("C:/Windows/Fonts/Arial.ttf");
		m_FpsText.setFont(m_Font);
		m_FpsText.setCharacterSize(15);
		m_FpsText.setFillColor(sf::Color::Yellow);
		m_FpsText.setPosition(m_WindowWidth - 30, 5);


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
		m_CanvasSprite.setTexture(m_CanvasTexture.getTexture());
		m_CanvasSprite.setPosition(m_WindowWidth / 2 - oiSize.x / 2, m_WindowHeight / 2 - oiSize.y / 2);
		
		// Prepare the genetic algorithm
		m_GeneticOptimizer.PrepareGA(m_OI, 125, 5, 15);
		
		// Prepare the Vertex array
		m_Va.resize(m_GeneticOptimizer.LinesSize() * 2);
		m_Va.setPrimitiveType(sf::Lines);
		for (unsigned int i = 0; i < m_Va.getVertexCount(); i++)
			m_Va[i].color = sf::Color::Black;

	}

	void Simulation::Render(const SolutionCandidate& solution)
	{
		m_Window.clear(sf::Color(125, 125, 125));
		m_Window.draw(m_OISprite);
		
		m_CanvasTexture.clear(sf::Color(150, 150, 150));
		
		sf::CircleShape c(20.0);
		c.setFillColor(sf::Color::Blue);
		c.setPosition(m_Test, 0.0);
		m_Test += 0.5;
		m_CanvasTexture.draw(c);

		unsigned int i = 0;
		for (const auto& line : solution.TransformedLines())
		{
			m_Va[i].position = line.first;
			m_Va[i + 1].position = line.second;
			i += 2;
		}

		m_CanvasTexture.draw(m_Va);
		m_CanvasTexture.display();
		
		m_Window.draw(m_CanvasSprite);
		
		//sf::Vertex xaxis[2] =
		//{
		//	sf::Vertex(sf::Vector2f(0.0f, m_WindowHeight / 2), sf::Color::Black),
		//	sf::Vertex(sf::Vector2f(m_WindowWidth, m_WindowHeight / 2), sf::Color::Black)
		//};

		//sf::Vertex yaxis[2] =
		//{
		//	sf::Vertex(sf::Vector2f(m_WindowWidth / 2, 0.0f), sf::Color::Black),
		//	sf::Vertex(sf::Vector2f(m_WindowWidth / 2, m_WindowHeight), sf::Color::Black)
		//};
		//
		//m_Window.draw(xaxis, 2, sf::Lines);
		//m_Window.draw(yaxis, 2, sf::Lines);


		// get fps
		int fps = static_cast<int>(1.0 / m_Clock.getElapsedTime().asSeconds());
		m_Clock.restart();

		// draw fps to the screen
		std::stringstream fpsSStream;
		fpsSStream << fps;
		m_FpsText.setString(fpsSStream.str());
		m_Window.draw(m_FpsText);
		m_Window.display();

	}

	void Simulation::Run()
	{
		m_Clock.restart();

		while (m_Running)
		{
			HandleEvents();
			if (!m_Paused)
			{
				auto solution = m_GeneticOptimizer.RunIteration();
				Render(solution);
				m_Paused = false;
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
