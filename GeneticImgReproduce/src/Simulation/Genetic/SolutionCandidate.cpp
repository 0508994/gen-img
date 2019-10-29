#include "SolutionCandidate.h"

namespace gir
{
	SolutionCandidate::SolutionCandidate() {}

	SolutionCandidate::~SolutionCandidate() {}

	SolutionCandidate::SolutionCandidate(std::vector<std::pair<sf::Vector2f, sf::Vector2f>>* lines, const Mat<Uint8>& threshEdges)
		:m_Fitness(0), m_LinesPtr(lines), m_Solution(threshEdges.Rows(), threshEdges.Cols()), m_TransformedLines(lines->size())
	{
		unsigned int size = m_LinesPtr->size();
		unsigned int rows = threshEdges.Rows();
		unsigned int cols = threshEdges.Cols();

		m_Translations.reserve(size);
		m_Rotations.reserve(size);

		std::random_device rd;										// obtain a random number from hardware
		std::mt19937 eng(rd());										// seed the generator
		std::uniform_real_distribution<float> distrr(0, rows);		// define the range for x-trans generator
		std::uniform_real_distribution<float> distrc(0, cols);		// define the range for y-trans generator
		std::uniform_real_distribution<float> distra(0.0, 360.0);	// define the range for the rotation angle generator
		
		// Generate initial transformations randomly
		for (unsigned int i = 0; i < size; i++)
		{
			m_Translations.emplace_back(sf::Vector2f(distrc(eng), distrr(eng)));
			m_Rotations.emplace_back(distra(eng));
		}
	
		ComputeSolution();
		ComputeFitness(threshEdges);
	}

	void SolutionCandidate::ComputeFitness(const Mat<Uint8>& threshEdges)
	{
		//assert(m_Solution.Cols() == threshEdges.Cols() && m_Solution.Rows() == threshEdges.Rows());

		m_Fitness = 0;	
		unsigned int nel = threshEdges.Cols() * threshEdges.Rows();

		// Directly access the pixel data
		Uint8* solPtr = m_Solution[0];
		const Uint8* threshEdgesPtr = threshEdges[0];

		// Count the matching pixels
		for (unsigned int i = 0; i < nel; i++)
			m_Fitness += (solPtr[i] == threshEdgesPtr[i]);
	}

	void SolutionCandidate::ComputeSolution()
	{
		sf::Transform translation, rotation, transform;

		m_Solution.Value(0);

		for (unsigned int i = 0; i < m_LinesPtr->size(); i++)
		{
			translation.translate(m_Translations[i]);
			rotation.rotate(m_Rotations[i]);
			transform = translation * rotation;
			
			m_TransformedLines[i].first = transform.transformPoint(m_TransformedLines[i].first);
			m_TransformedLines[i].second = transform.transformPoint(m_TransformedLines[i].second);

			BresenhamsLine(m_TransformedLines[i]);
		}
	}

	void SolutionCandidate::BresenhamsLine(const std::pair<sf::Vector2f, sf::Vector2f>& line)
	{
		//int rows = static_cast<int>(m_Solution.Rows());
		//int cols = static_cast<int>(m_Solution.Cols());

		int x0 = static_cast<int>(line.first.x);
		int x1 = static_cast<int>(line.second.x);
		int y0 = static_cast<int>(line.first.y);
		int y1 = static_cast<int>(line.second.y);

		int dx, dy, p, x, y;

		dx = x1 - x0;
		dy = y1 - y0;

		x = x0;
		y = y0;

		p = 2 * dy - dx;

		while (x < x1)
		{
			if (p >= 0)
			{
				m_Solution[y][x] = 255;
				y = y + 1;
				p = p + 2 * dy - 2 * dx;
			}
			else
			{
				m_Solution[y][x] = 255;
				p = p + 2 * dy;
			}
			x = x + 1;
		}
	}
}