#pragma once
#include <vector>
#include <sstream>
#include "RNG.h"
#include "SolutionCandidate.h"
#include "ImgOps.h"


namespace gir
{
	class GeneticOptimizer
	{
	private:
		unsigned int m_Iteration;
		unsigned int m_PopSize;
		unsigned int m_Elitismn;
		double m_TransMutChance;
		double m_RotMutChance;

		std::vector<SolutionCandidate> m_Population;

		Mat<Uint8> m_ThreshEdges;
		std::vector<Line> m_Lines;
		std::shared_ptr<RNG> m_Rng;

		unsigned int m_ImgRows;
		unsigned int m_ImgCols;
	public:
		GeneticOptimizer();
		GeneticOptimizer(unsigned int popSize, double transMutChance, double rotMutChance, unsigned int elitismn);
		~GeneticOptimizer();

		void PrepareGA(const sf::Image& origImage, Uint8 threshold, unsigned int minLineLen, unsigned int maxLineLen);
		const SolutionCandidate& RunIterations(unsigned int nIterations);
		std::string GetInfo() const;

		inline const std::vector<Line>& Lines() const { return m_Lines; }
		inline unsigned int LinesSize() const { return m_Lines.size(); }
	private:
		std::pair<const SolutionCandidate*,  const SolutionCandidate*> Selection(const std::vector<double>& weights);
	};
}

