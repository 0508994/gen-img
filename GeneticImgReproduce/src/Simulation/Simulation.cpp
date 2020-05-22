#include "Simulation.h"

#define FPS 60

namespace gir
{
	Simulation::Simulation(unsigned int width, unsigned int height, unsigned int popSize,
						   double transMutChance, double rotMutChance, unsigned int elitismn)
		: m_WindowWidth(width)
		, m_WindowHeight(height)
		, m_Window(sf::VideoMode(width, height), "Image Reconstruction", sf::Style::Titlebar | sf::Style::Close)
		, m_GeneticOptimizer(popSize, transMutChance, rotMutChance, elitismn)
		, m_ItersToRun(1)
	{
		m_Window.setFramerateLimit(FPS);

		m_Font.loadFromFile("C:/Windows/Fonts/Calibri.ttf");

		m_Background[0] = sf::Vertex(sf::Vector2f(0, m_WindowHeight), sf::Color(125, 125, 125));
		m_Background[1] = sf::Vertex(sf::Vector2f(0, 0), sf::Color(125, 125, 125));
		m_Background[2] = sf::Vertex(sf::Vector2f(m_WindowWidth, 0), sf::Color(225, 225, 225));
		m_Background[3] = sf::Vertex(sf::Vector2f(m_WindowWidth, m_WindowHeight), sf::Color(225, 225, 225));

		m_Running = true;
		m_Paused  = false;
	}
	
	void Simulation::Prepare(std::string file, unsigned int threshold, unsigned int minLineLen, unsigned int maxLineLen)
	{
		// Prepare the original sprite
		m_OI.loadFromFile(file);
		
		//Mat<Uint8> gray(m_OI.getSize().y, m_OI.getSize().x);
		//Mat<Uint8> edge(m_OI.getSize().y, m_OI.getSize().x);
		//ToGrayscale(m_OI, gray);
		//Sobel(gray, edge);
		//Threshold(edge, 125);
		//Canny(gray, edge, 0.5, 180, 220);
		//ToSFMLImage(edge, m_OI);

		sf::Vector2u oiSize = m_OI.getSize();
		float newImgWidth = 0.30 * m_WindowWidth;
		float newImgHeight = 0.30 * m_WindowHeight;
		float aspectRatio = oiSize.x / oiSize.y;
		float newAspectRatio = newImgWidth / newImgHeight;
		float scaleFactor = newAspectRatio > aspectRatio ? newImgHeight / oiSize.y : newImgWidth / oiSize.x;

		m_OITexture.loadFromImage(m_OI);
		m_OISprite.setTexture(m_OITexture);
		m_OISprite.setScale(scaleFactor, scaleFactor);

		m_CanvasTexture.create(oiSize.x, oiSize.y);
		m_CanvasSprite.setTexture(m_CanvasTexture.getTexture());
		m_CanvasSprite.setPosition(m_WindowWidth / 2 - oiSize.x / 2, m_WindowHeight / 2 - oiSize.y / 2);
		
		// Prepare the genetic algorithm
		m_GeneticOptimizer.PrepareGA(m_OI, threshold, minLineLen, maxLineLen);
		
		// Prepare the Vertex array
		m_Va.resize(m_GeneticOptimizer.LinesSize() * 2);
		m_Va.setPrimitiveType(sf::Lines);
		for (unsigned int i = 0; i < m_Va.getVertexCount(); i++)
			m_Va[i].color = sf::Color(44, 43, 60);

		m_Info.setFont(m_Font);
		m_Info.setCharacterSize(16);
		m_Info.setStyle(sf::Text::Bold);
		m_Info.setFillColor(sf::Color(44, 43, 60));
		m_Info.setPosition(5, oiSize.y * scaleFactor + 10);
	}

	void Simulation::Render(const SolutionCandidate& solution)
	{
		m_Window.clear();
		m_Window.draw(m_Background, 4, sf::Quads);
		m_Window.draw(m_OISprite);
		
		m_CanvasTexture.clear(sf::Color::Transparent);

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
		

		// get fps
		int fps = static_cast<int>(1.0 / m_Clock.getElapsedTime().asSeconds());
		m_Clock.restart();

		std::stringstream ss;
		ss << "Framerate: " << fps << "\n";
		ss << "Iterations to run at once: " << m_ItersToRun << "\n";
		ss << m_GeneticOptimizer.GetInfo();
		m_Info.setString(ss.str());
		m_Window.draw(m_Info);

		// draw info string to the screen
		m_Window.draw(m_Info);

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
				const auto& solution = m_GeneticOptimizer.RunIterations(m_ItersToRun);
				Render(solution);
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
			{
				m_Running = false;
			}
			else if (e.type == sf::Event::KeyPressed)
			{
				switch (e.key.code)
				{
					case sf::Keyboard::Up:
						m_ItersToRun++;
						m_ItersToRun = m_ItersToRun > 10 ? 1 : m_ItersToRun;
						break;
					case sf::Keyboard::Down:
						m_ItersToRun--;
						m_ItersToRun = m_ItersToRun > 0 ? m_ItersToRun : 10;
					default:
						break;
				}
			}
		}

	}
}
