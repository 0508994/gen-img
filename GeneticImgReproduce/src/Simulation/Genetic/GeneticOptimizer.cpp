#include "GeneticOptimizer.h"

namespace gir
{
	GeneticOptimizer::GeneticOptimizer() :m_PopSize(0) {}

	GeneticOptimizer::~GeneticOptimizer() {}

	GeneticOptimizer::GeneticOptimizer(unsigned int popSize, double transMutChance, double rotMutChance, unsigned int elitismn)
		:m_PopSize(popSize),
		m_Elitismn(elitismn),
		m_TransMutChance(transMutChance),
		m_RotMutChance(rotMutChance),
		m_Generator((std::random_device())())
	{
		m_Population.reserve(m_PopSize);
	}

	void GeneticOptimizer::PrepareGA(const sf::Image& origImage, Uint8 threshold, unsigned int minLineLen, unsigned int maxLineLen)
	{
		assert(maxLineLen > minLineLen);

		auto size = origImage.getSize();
		Mat<Uint8> gray(size.y, size.x);
		m_ThreshEdges.Resize(size.y, size.x);

		ToGrayscale(origImage, gray);
		Sobel(gray, m_ThreshEdges);
		Threshold(m_ThreshEdges, threshold);

		std::uniform_int_distribution<unsigned int> lineLenDistr(minLineLen, maxLineLen);

		int whitePixelCount = m_ThreshEdges.ValueCount(255);
		assert(whitePixelCount > 0);

		int lineLen, halfLineLen;

		while (whitePixelCount > 0)
		{
			// All lines are zero centered with y0=y1=0

			lineLen = lineLenDistr(m_Generator);
			halfLineLen = lineLen / 2;

			whitePixelCount -= lineLen;
			if (whitePixelCount - lineLen < 0)
				break;

			m_Lines.emplace_back(std::make_pair(sf::Vector2f(-halfLineLen, 0.0f), sf::Vector2f(halfLineLen, 0.0f)));
		}

		for (const auto& line : m_Lines)
			std::cout << line.first.x << "," << line.first.y << "\t" << line.second.x << "," << line.second.y << std::endl;
		
		for (unsigned int i = 0; i < m_PopSize; i++)
			m_Population.emplace_back(SolutionCandidate(&m_Lines, m_ThreshEdges));

		for (const auto& sc : m_Population)
			std::cout << sc.GetFitness() << "\t";
	}

	std::pair<const SolutionCandidate*, const SolutionCandidate*> GeneticOptimizer::Selection(const std::vector<double>& probas) 
	{
		int i1, i2;
		std::discrete_distribution<int> distr(probas.begin(), probas.end());
		
		//auto& generator = m_Generator;
		//std::generate(indices[0], indices[1], [&generator, &distr]() { return distr(generator); });
		
		i1 = distr(m_Generator);	
		//i2 must be different then i1
		do
		{
			i2 = distr(m_Generator);
		} while (i1 == i2);

		return std::make_pair(&m_Population[i1], &m_Population[i2]);
		//return std::make_pair(nullptr, nullptr);
	}
}