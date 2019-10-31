#include "SolutionCandidate.h"

namespace gir
{	
	std::mt19937 SolutionCandidate::s_Generator((std::random_device())());

	SolutionCandidate::SolutionCandidate() {}

	SolutionCandidate::~SolutionCandidate() {}

	SolutionCandidate::SolutionCandidate(std::vector<std::pair<sf::Vector2f, sf::Vector2f>>* lines, const Mat<Uint8>& threshEdges)
		:m_Fitness(0), m_LinesSize(lines->size()), m_LinesPtr(lines), m_Solution(threshEdges.Rows(), threshEdges.Cols()), m_TransformedLines(lines->size())
	{
		float rows = static_cast<float>(threshEdges.Rows());
		float cols = static_cast<float>(threshEdges.Cols());

		m_Translations.reserve(m_LinesSize);
		m_Rotations.reserve(m_LinesSize);

		std::uniform_real_distribution<float> distrr(0.0f, rows);		// define the range for x-trans generator
		std::uniform_real_distribution<float> distrc(0.0f, cols);		// define the range for y-trans generator
		std::uniform_real_distribution<float> distra(0.0f, 360.0f);		// define the range for the rotation angle generator
		
		// Generate initial transformations randomly
		for (unsigned int i = 0; i < m_LinesSize; i++)
		{
			m_Translations.emplace_back(sf::Vector2f(distrc(s_Generator), distrr(s_Generator)));
			m_Rotations.emplace_back(distra(s_Generator));
		}
	
		ComputeSolution();
		ComputeFitness(threshEdges);
	}

	SolutionCandidate::SolutionCandidate(const SolutionCandidate& other)
		:m_LinesPtr(other.m_LinesPtr),
		m_LinesSize(other.m_LinesSize),
		m_Solution(other.m_Solution.Rows(), other.m_Solution.Cols()),
		m_Rotations(other.m_Rotations),
		m_Translations(other.m_Translations)
	{
		// No need to copy m_Solution matrix just allocate the space
	}

	SolutionCandidate::SolutionCandidate(SolutionCandidate&& other)
		:m_LinesPtr(other.m_LinesPtr),
		m_Fitness(other.m_Fitness)
	{
		m_Rotations = std::move(other.m_Rotations);
		m_Translations = std::move(other.m_Translations);
		m_TransformedLines = std::move(other.m_TransformedLines);
		m_Solution = std::move(other.m_Solution);

		other.m_LinesPtr = nullptr;
	}

	SolutionCandidate& SolutionCandidate::operator=(SolutionCandidate&& other)
	{
		m_LinesPtr = other.m_LinesPtr;
		m_Fitness = other.m_Fitness;

		m_Rotations = std::move(other.m_Rotations);
		m_Translations = std::move(other.m_Translations);
		m_TransformedLines = std::move(other.m_TransformedLines);
		m_Solution = std::move(other.m_Solution);

		other.m_LinesPtr = nullptr;

		return *this;
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
			m_Fitness += (solPtr[i] == 255 && 255 == threshEdgesPtr[i]); // count only the matching white pixels
	}

	void SolutionCandidate::ComputeSolution()
	{
		sf::Transform translation, rotation, transform;
		auto& lines = *m_LinesPtr;
		
		m_Solution.Value(0);

		for (unsigned int i = 0; i < m_LinesSize; i++)
		{
			translation.translate(m_Translations[i]);
			rotation.rotate(m_Rotations[i]);
			transform = translation * rotation;
			
			m_TransformedLines[i].first = transform.transformPoint(lines[i].first);
			m_TransformedLines[i].second = transform.transformPoint(lines[i].second);

			ClampLine(m_TransformedLines[i]);
			BresenhamsLine(m_TransformedLines[i]);
		}
	}

	void SolutionCandidate::Crossover(const SolutionCandidate& parent1, const SolutionCandidate& parent2, SolutionCandidate& child1, SolutionCandidate& child2)
	{	
		// child1, and child2 must be constructed before calling this function as copies of their respective parents
		static std::uniform_int_distribution<unsigned int> distr(1, parent1.m_LinesSize - 1);
		unsigned int splitIndex = distr(s_Generator);

		for (unsigned int i = splitIndex; i < parent1.m_LinesSize; i++)
		{
			child1.m_Translations[i] = parent2.m_Translations[i];
			child1.m_Rotations[i] = parent2.m_Rotations[i];
			
			child2.m_Translations[i] = parent1.m_Translations[i];
			child2.m_Rotations[i] = parent1.m_Rotations[i];
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

		line.first.x = Clamp(line.first.x, 0.0f, cols - 1);
		line.second.x = Clamp(line.second.x, 0.0f, cols - 1);
		line.first.y = Clamp(line.first.y, 0.0f, rows - 1);
		line.second.y = Clamp(line.second.y, 0.0f, rows - 1);
	}

	void SolutionCandidate::BresenhamsLine(const std::pair<sf::Vector2f, sf::Vector2f>& line)
	{
		int x0 = static_cast<int>(line.first.x);
		int x1 = static_cast<int>(line.second.x);
		int y0 = static_cast<int>(line.first.y);
		int y1 = static_cast<int>(line.second.y);

		if (x1 < x0) std::swap(x1, x0);
		if (y1 < y0) std::swap(y1, y0);

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