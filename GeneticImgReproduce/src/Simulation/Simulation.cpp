#include "Simulation.h"

#define FPS 60

namespace gir
{
	Simulation::Simulation() {}
	
	Simulation::Simulation(unsigned int width, unsigned int height)
		:m_WindowWidth(width),
		 m_WindowHeight(height),
		 m_Window(sf::VideoMode(width, height), "Image Reconstruction")
	{
		m_Window.setFramerateLimit(FPS);
		m_Running = true;
	}

	Simulation::~Simulation() {}

	void Simulation::Render()
	{
		if (m_Redraw)
		{
			sf::Image i;
			i.loadFromFile("../img/wikipedia_sobel.png");

			Mat<sf::Uint8> gray(i.getSize().y, i.getSize().x);
			Mat<sf::Uint8> edge(i.getSize().y, i.getSize().x);

			ToGrayscale(i, gray);
			Scharr(gray, edge);
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
