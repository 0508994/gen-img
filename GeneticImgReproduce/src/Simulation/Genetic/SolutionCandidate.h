#pragma once
#include "ImgOps.h"
#include "RNG.h"
#include "SFML/Graphics/Transform.hpp"

namespace gir
{
	class SolutionCandidate
	{
	private:
		int m_Fitness = 0;
		unsigned int m_LinesSize;
		std::vector<Line>* m_LinesPtr;
		std::vector<sf::Vector2f> m_Translations;
		std::vector<double> m_Rotations;
		
		std::shared_ptr<RNG> m_Rng;
	public:
		SolutionCandidate();
		SolutionCandidate(std::vector<Line>* lines, const Mat<Uint8>& threshEdges, const std::shared_ptr<RNG>& rng);
		SolutionCandidate(std::vector<Line>* lines, unsigned int solutionRows, unsigned int solutionCols, const std::shared_ptr<RNG>& rng);
		SolutionCandidate(const SolutionCandidate& other);
		SolutionCandidate(SolutionCandidate&& other);
		~SolutionCandidate();

		SolutionCandidate& operator=(SolutionCandidate&& other);

		void ComputeFitness(const Mat<Uint8>& threshEdges);
		static void Crossover(const SolutionCandidate& parent1, const SolutionCandidate& parent2, SolutionCandidate& child1, SolutionCandidate& child2);
		void Mutate(double transMutChance, double rotMutChance);
		
		inline int GetFitness() const { return m_Fitness; }
		inline bool operator< (const SolutionCandidate& other) const { return m_Fitness < other.m_Fitness; }
		inline bool operator> (const SolutionCandidate& other) const { return m_Fitness > other.m_Fitness; }
		inline bool operator<= (const SolutionCandidate& other) const { return m_Fitness <= other.m_Fitness; }
		inline bool operator>= (const SolutionCandidate& other) const { return m_Fitness >= other.m_Fitness; }

		std::vector<Line> TransformedLines() const;
	private:
		void ClampLine(Line& line, unsigned int rows, unsigned int columns) const;
		bool WithinBounds(const Line& line, unsigned int rows, unsigned int columns) const;
	};
}
