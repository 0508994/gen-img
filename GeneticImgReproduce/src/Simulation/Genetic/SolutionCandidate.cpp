#include "SolutionCandidate.h"

namespace gir
{	
	SolutionCandidate::SolutionCandidate() {}

	SolutionCandidate::~SolutionCandidate() {}

	SolutionCandidate::SolutionCandidate(std::vector<Line>* lines, const Mat<Uint8>& threshEdges, std::shared_ptr<RNG> rng)
		: m_Fitness(0)
		, m_LinesSize(lines->size())
		, m_LinesPtr(lines)
		, m_Rng(rng)
	{
		float rows = static_cast<float>(threshEdges.Rows());
		float cols = static_cast<float>(threshEdges.Cols());

		m_Translations.reserve(m_LinesSize);
		m_Rotations.reserve(m_LinesSize);
		
		// Generate initial transformations randomly
		for (unsigned int i = 0; i < m_LinesSize; i++)
		{
			m_Translations.emplace_back(sf::Vector2f(m_Rng->RandomX(), m_Rng->RandomY()));
			m_Rotations.emplace_back(m_Rng->RandomAngle());
		}
	
		ComputeFitness(threshEdges);
	}

	SolutionCandidate::SolutionCandidate(std::vector<Line>* lines, unsigned int solutionRows, unsigned int solutionCols, std::shared_ptr<RNG> rng)
		: m_Fitness(0)
		, m_LinesSize(lines->size())
		, m_LinesPtr(lines)
		, m_Rng(rng)
	{
		m_Translations.reserve(m_LinesSize);
		m_Rotations.reserve(m_LinesSize);
	}

	SolutionCandidate::SolutionCandidate(const SolutionCandidate& other)
		: m_Fitness(other.m_Fitness)
		, m_LinesSize(other.m_LinesSize)
		, m_LinesPtr(other.m_LinesPtr)
		, m_Translations(other.m_Translations)
		, m_Rotations(other.m_Rotations)
		, m_Rng(other.m_Rng)
	{
	}

	SolutionCandidate::SolutionCandidate(SolutionCandidate&& other)
		: m_Fitness(other.m_Fitness)
		, m_LinesSize(other.m_LinesSize)
		, m_LinesPtr(other.m_LinesPtr)
		, m_Rng(other.m_Rng)
	{
		m_Rotations		= std::move(other.m_Rotations);
		m_Translations	= std::move(other.m_Translations);

		other.m_LinesPtr = nullptr;
	}

	SolutionCandidate& SolutionCandidate::operator=(SolutionCandidate&& other)
	{
		m_Fitness	= other.m_Fitness;
		m_LinesPtr	= other.m_LinesPtr;
		m_LinesSize = other.m_LinesSize;
		m_Fitness	= other.m_Fitness;
		m_Rng		= other.m_Rng;

		m_Rotations		= std::move(other.m_Rotations);
		m_Translations	= std::move(other.m_Translations);

		other.m_LinesPtr = nullptr;

		return *this;
	}

	void SolutionCandidate::ComputeFitness(const Mat<Uint8>& threshEdges)
	{
		//assert(m_Solution.Cols() == threshEdges.Cols() && m_Solution.Rows() == threshEdges.Rows());
		
		auto& lines = *m_LinesPtr;
		unsigned int rows = threshEdges.Rows(), cols = threshEdges.Cols();
		int dx, dy, p, x, y, x0, x1, y0, y1;
		bool *used = new bool[rows * cols];
		
		memset(used, 0, rows * cols);
		m_Fitness = 0;

		for (unsigned int i = 0; i < m_LinesSize; i++)
		{
			sf::Transform transform;

			transform.rotate(m_Rotations[i]);
			transform.translate(m_Translations[i]);

			Line transfLine = std::make_pair(
				transform.transformPoint(lines[i].first),
				transform.transformPoint(lines[i].second));

			ClampLine(transfLine, rows, cols);

			// Compare pixels from the thresholded edges to the ones
			// computed from the Bressenhams line drawing algorithm
			x0 = static_cast<int>(transfLine.first.x);
			x1 = static_cast<int>(transfLine.second.x);
			y0 = static_cast<int>(transfLine.first.y);
			y1 = static_cast<int>(transfLine.second.y);

			if (x1 < x0) std::swap(x1, x0);
			if (y1 < y0) std::swap(y1, y0);

			dx = x1 - x0;
			dy = y1 - y0;

			x = x0;
			y = y0;

			p = 2 * dy - dx;

			while (x < x1)
			{
				if (p >= 0)
				{
					if (threshEdges[y][x] == 255 && !used[y * cols + x])
					{
						m_Fitness++;
						used[y * cols + x] = true;
					}
					else if (used[y * cols + x] && m_Fitness != 0)
						m_Fitness--;
					
					y = y + 1;
					p = p + 2 * dy - 2 * dx;
				}
				else
				{
					if (threshEdges[y][x] == 255 && !used[y * cols + x])
					{
						m_Fitness++;
						used[y * cols + x] = true;
					}
					else if (used[y * cols + x] && m_Fitness != 0)
						m_Fitness--;

					p = p + 2 * dy;
				}
				x = x + 1;
			}
		}

		delete[] used;
	}

	void SolutionCandidate::Crossover(const SolutionCandidate& parent1, const SolutionCandidate& parent2, SolutionCandidate& child1, SolutionCandidate& child2)
	{	
		auto rng = parent1.m_Rng;
		unsigned int splitIndex = rng->RandomSplitIndex();
		
		for (unsigned int i = 0; i < splitIndex; i++)
		{
			child1.m_Translations.push_back(parent1.m_Translations[i]);
			child1.m_Rotations.push_back(parent1.m_Rotations[i]);

			child2.m_Translations.push_back(parent2.m_Translations[i]);
			child2.m_Rotations.push_back(parent2.m_Rotations[i]);
		}
		
		for (unsigned int i = splitIndex; i < parent1.m_LinesSize; i++)
		{
			child1.m_Translations.push_back(parent2.m_Translations[i]);
			child1.m_Rotations.push_back(parent2.m_Rotations[i]);
			
			child2.m_Translations.push_back(parent1.m_Translations[i]);
			child2.m_Rotations.push_back(parent1.m_Rotations[i]);
		}
	}

	void SolutionCandidate::Mutate(double transMutChance, double rotMutChance)
	{
		if (m_Rng->Probability() <= transMutChance)
		{
			// Mutate the random number of line translations 
			for (unsigned int i = 0; i < m_Rng->RandomLineNumber(); i++)
				m_Translations[m_Rng->RandomLineIndex()] = sf::Vector2f(m_Rng->RandomX(), m_Rng->RandomY());
				//m_Translations[m_Rng->RandomLineIndex()] += sf::Vector2f(m_Rng->RandomIncr(), m_Rng->RandomIncr());
		}

		if (m_Rng->Probability() <= rotMutChance)
		{
			// Mutate the random number of line rotations
			for (unsigned int i = 0; i < m_Rng->RandomLineNumber(); i++)
				//m_Rotations[m_Rng->RandomLineIndex()] = m_Rng->RandomAngle();
				m_Rotations[m_Rng->RandomLineIndex()] += m_Rng->RandomIncr() * 2;
		}
	}

	std::vector<Line> SolutionCandidate::TransformedLines() const
	{
		auto& lines = *m_LinesPtr;
		std::vector<Line> result;
		
		result.reserve(m_LinesSize);

		for (unsigned int i = 0; i < m_LinesSize; i++)
		{
			sf::Transform transform;

			transform.rotate(m_Rotations[i]);
			transform.translate(m_Translations[i]);

			result.emplace_back(std::make_pair(
				transform.transformPoint(lines[i].first),
				transform.transformPoint(lines[i].second)));
		}

		return result;
	}

	void SolutionCandidate::ClampLine(Line& line, unsigned int rows, unsigned int columns) const
	{
		float rowsf = static_cast<int>(rows);
		float colsf = static_cast<int>(columns);

		line.first.x = std::clamp(line.first.x, 0.0f, colsf - 1);
		line.second.x = std::clamp(line.second.x, 0.0f, colsf - 1);
		line.first.y = std::clamp(line.first.y, 0.0f, rowsf - 1);
		line.second.y = std::clamp(line.second.y, 0.0f, rowsf - 1);
	}

	bool SolutionCandidate::WithinBounds(const Line& line, unsigned int rows, unsigned int columns) const
	{
		float rowsf = static_cast<int>(rows);
		float colsf = static_cast<int>(columns);

		if (line.first.x < 0 || line.first.x >= colsf ||
			line.second.x < 0 || line.second.x >= colsf ||
			line.first.y < 0 || line.first.y >= rowsf ||
			line.second.y < 0 || line.second.y >= rowsf)
			return false;

		return true;
	}
}