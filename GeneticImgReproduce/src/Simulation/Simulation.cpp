#include "Simulation.h"

#define FPS 60

namespace gir
{
	Simulation::Simulation() {}
	
	Simulation::~Simulation() {}
	
	Simulation::Simulation(unsigned int width, unsigned int height)
		:m_WindowWidth(width),
		m_WindowHeight(height),
		m_Window(sf::VideoMode(width, height), "Image Reconstruction"),
		m_GeneticOptimizer(20, 0.001, 0.001, 2)
	{
		m_Window.setFramerateLimit(FPS);
		m_Running = true;
	}
	
	void Simulation::Prepare()
	{
		sf::Image i;
		i.loadFromFile("../img/tower_resized.jpg");
		m_GeneticOptimizer.PrepareGA(i, 125, 5, 15);
	}

	void Simulation::Render()
	{
		if (m_Redraw)
		{
			sf::Image i;
			//i.loadFromFile("../img/wikipedia_sobel.png");
			//i.loadFromFile("../img/canny_blur_test.png");
			i.loadFromFile("../img/tower_resized.jpg");

			Mat<sf::Uint8> gray(i.getSize().y, i.getSize().x);
			Mat<sf::Uint8> edge(i.getSize().y, i.getSize().x);

			ToGrayscale(i, gray);
			Sobel(gray, edge);
			Threshold(edge, 125);
			//Canny(gray, edge, 0.5, 50, 255);
			//std::cout << edge.ValueCount(255) << std::endl;;
			ToSFMLImage(edge, i);

			sf::Texture t;
			t.loadFromImage(i);

			sf::Sprite s(t);
			s.scale(sf::Vector2f(0.7f, 0.7f));

			m_Window.clear();
			m_Window.draw(s);
			m_Window.display();
			m_Redraw = false;
		}
	}

	void Simulation::Run()
	{
		while (m_Running)
		{
			HandleEvents();
			// gen algo stuff goes here
			Render();
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
