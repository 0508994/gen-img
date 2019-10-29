#include "SolutionCandidate.h"

namespace gir
{
	std::random_device SolutionCandidate::s_Rd;				// obtain a random number from hardware
	
	std::mt19937 SolutionCandidate::s_Generator(s_Rd());	// seed the generator

	SolutionCandidate::SolutionCandidate() {}

	SolutionCandidate::~SolutionCandidate() {}

	SolutionCandidate::SolutionCandidate(std::vector<std::pair<sf::Vector2f, sf::Vector2f>>* lines, const Mat<Uint8>& threshEdges)
		:m_Fitness(0), m_LinesSize(lines->size()), m_LinesPtr(lines), m_Solution(threshEdges.Rows(), threshEdges.Cols()), m_TransformedLines(lines->size())
	{
		unsigned int rows = threshEdges.Rows();
		unsigned int cols = threshEdges.Cols();

		m_Translations.reserve(m_LinesSize);
		m_Rotations.reserve(m_LinesSize);

		std::uniform_real_distribution<float> distrr(0, rows);		// define the range for x-trans generator
		std::uniform_real_distribution<float> distrc(0, cols);		// define the range for y-trans generator
		std::uniform_real_distribution<float> distra(0.0, 360.0);	// define the range for the rotation angle generator
		
		// Generate initial transformations randomly
		for (unsigned int i = 0; i < m_LinesSize; i++)
		{
			m_Translations.emplace_back(sf::Vector2f(distrc(s_Generator), distrr(s_Generator)));
			m_Rotations.emplace_back(distra(s_Generator));
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

		for (unsigned int i = 0; i < m_LinesSize; i++)
		{
			translation.translate(m_Translations[i]);
			rotation.rotate(m_Rotations[i]);
			transform = translation * rotation;
			
			m_TransformedLines[i].first = transform.transformPoint(m_TransformedLines[i].first);
			m_TransformedLines[i].second = transform.transformPoint(m_TransformedLines[i].second);

			ClampLine(m_TransformedLines[i]);
			BresenhamsLine(m_TransformedLines[i]);
		}
	}

	void SolutionCandidate::Crossover(SolutionCandidate& other)
	{									
		static std::uniform_int_distribution<unsigned int> distr(1, m_LinesSize - 1);
		unsigned int splitIndex = distr(s_Generator);

		for (unsigned int i = splitIndex; i < m_LinesSize; i++)
		{
			std::swap(m_Translations[i], other.m_Translations[i]);
			std::swap(m_Rotations[i], other.m_Rotations[i]);
		}
	}

	void SolutionCandidate::Mutate(double transMutChance, double rotMutChance)
	{
		static std::uniform_real_distribution<double> unif(0.0, 1.0);						 // probability generator
		static std::uniform_int_distribution<unsigned int> distrMutLin(1, m_LinesSize / 4);  // number of mutations generator
		static std::uniform_int_distribution<unsigned int> distrInd(1, m_LinesSize - 1);     // index generator
		static std::uniform_real_distribution<float> distrr(0, m_Solution.Rows());           // random y-translation generator
		static std::uniform_real_distribution<float> distrc(0, m_Solution.Cols());			 // random x-translation generator
		static std::uniform_real_distribution<float> distra(0.0, 360.0);					 // define the range for the rotation angle generator

		if (unif(s_Generator) <= transMutChance)
		{
			// Mutate the random number of line translations between [1, m_LinesSize / 4]
			// Replace the translation vectors with randomly generated ones
			for (unsigned int i = 0; i < distrMutLin(s_Generator); i++)
				m_Translations[distrInd(s_Generator)] = sf::Vector2f(distrc(s_Generator), distrr(s_Generator));
		}

		if (unif(s_Generator) <= rotMutChance)
		{
			// Mutate the random number of line rotations between [1, m_LinesSize / 4]
			// Replace the rotation angles with randomly generated ones
			for (unsigned int i = 0; i < distrMutLin(s_Generator); i++)
				m_Rotations[distrInd(s_Generator)] = distra(s_Generator);		
		}
	}

	void SolutionCandidate::ClampLine(std::pair<sf::Vector2f, sf::Vector2f>& line)
	{
		float rows = static_cast<int>(m_Solution.Rows());
		float cols = static_cast<int>(m_Solution.Cols());

		line.first.x = Clamp(line.first.x, 0.0f, cols);
		line.second.x = Clamp(line.second.x, 0.0f, cols);
		line.first.y = Clamp(line.first.y, 0.0f, rows);
		line.second.y = Clamp(line.second.y, 0.0f, rows);
	}

	void SolutionCandidate::BresenhamsLine(const std::pair<sf::Vector2f, sf::Vector2f>& line)
	{
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