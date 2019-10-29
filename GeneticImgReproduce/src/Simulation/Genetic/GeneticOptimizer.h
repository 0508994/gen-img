#pragma once
#include <vector>
#include "SolutionCandidate.h"
#include "ImgOps.h"


namespace gir
{
	class GeneticOptimizer
	{
	private:
		unsigned int m_PopSize;
		double m_TransMutChance;
		double m_RotMutChance;
		std::vector<SolutionCandidate> m_Population;

		Mat<Uint8> m_ThreshEdges;
		std::vector<std::pair<sf::Vector2f, sf::Vector2f>> m_Lines;
		unsigned int m_WhitePixelCount;
		unsigned int m_MinLineLen;
		unsigned int m_MaxLineLen;
	public:
		GeneticOptimizer();
		GeneticOptimizer(unsigned int popSize, double transMutChance, double rotMutChance, unsigned int minLineLen, unsigned int maxLineLen);

		void PrepareGA(const sf::Image& origImage, Uint8 threshold);

		~GeneticOptimizer();
	};
}

