#include "Simulation.h"

#define FPS 60

namespace gir
{
	Simulation::Simulation() {}
	
	Simulation::Simulation(unsigned int width, unsigned int height)
		:m_WindowWidth(width),
		 m_WindowHeight(height),
		 m_Window(sf::VideoMode(width, height), "Image reconstruction")
	{
		m_Window.setFramerateLimit(FPS);
		m_Running = true;
	}

	Simulation::~Simulation() {}

	void Simulation::Render()
	{
		sf::CircleShape shape(100.0f);
		shape.setFillColor(sf::Color::Red);

		m_Window.clear();
		m_Window.draw(shape);
		m_Window.display();
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
