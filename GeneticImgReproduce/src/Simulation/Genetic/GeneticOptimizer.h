#pragma once
#include <vector>
#include <list>
#include "SolutionCandidate.h"
#include "ImgOps.h"


namespace gir
{
	class GeneticOptimizer
	{
	private:
		unsigned int m_PopSize;
		unsigned int m_Elitismn;
		double m_TransMutChance;
		double m_RotMutChance;
		std::mt19937 m_Generator;

		std::vector<SolutionCandidate> m_Population;
		//std::set<SolutionCandidate, std::greater<SolutionCandidate>> m_Population;
		//std::list<SolutionCandidate> m_Population;

		Mat<Uint8> m_ThreshEdges;
		std::vector<std::pair<sf::Vector2f, sf::Vector2f>> m_Lines;
	public:
		GeneticOptimizer();
		GeneticOptimizer(unsigned int popSize, double transMutChance, double rotMutChance, unsigned int elitismn);

		void PrepareGA(const sf::Image& origImage, Uint8 threshold, unsigned int minLineLen, unsigned int maxLineLen);
		const SolutionCandidate& RunIteration();

		~GeneticOptimizer();
	private:
		std::pair<const SolutionCandidate*,  const SolutionCandidate*> Selection(const std::vector<double>& probas);
	};
}

