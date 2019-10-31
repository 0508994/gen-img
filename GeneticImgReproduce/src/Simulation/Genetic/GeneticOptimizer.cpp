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

	const SolutionCandidate& GeneticOptimizer::RunIteration()
	{
		double accFitness = 0.0;
		std::vector<SolutionCandidate> newPopulation;
		std::vector<double> probas(m_PopSize);

		// Copy the best solutions from the last iteration
		newPopulation.reserve(m_PopSize);
		for (unsigned i = 0; i < m_Elitismn; i++)
			newPopulation.emplace_back(m_Population[i]);

		while (newPopulation.size() < m_PopSize)
		{
			// Calculate the probability distribution
			for (const auto& sc : m_Population)
				accFitness += sc.GetFitness();	
			for (unsigned int i = 0; i < m_PopSize; i++)
				probas[i] = m_Population[i].GetFitness() / accFitness;

			// Perform the selection
			auto parents = Selection(probas);
			auto& parent1 = *(parents.first);
			auto& parent2 = *(parents.second);

			// Crossover
			SolutionCandidate child1(parent1), child2(parent2);
			SolutionCandidate::Crossover(parent1, parent2, child1, child2);

			// Mutate the children
			child1.Mutate(m_TransMutChance, m_RotMutChance);
			child2.Mutate(m_TransMutChance, m_RotMutChance);

			// Calculate the fitness of the new units
			child1.ComputeFitness(m_ThreshEdges);
			child2.ComputeFitness(m_ThreshEdges);

			// Add the children to the new population
			newPopulation.emplace_back(std::move(child1));
			newPopulation.emplace_back(std::move(child2));
		}

		m_Population.swap(newPopulation);
		std::sort(m_Population.begin(), m_Population.end(), std::greater<SolutionCandidate>());

		return m_Population[0];
	}
}