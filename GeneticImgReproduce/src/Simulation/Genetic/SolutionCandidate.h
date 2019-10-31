#pragma once
#include "ImgOps.h"
#include "RNG.h"
#include "SFML/Graphics/Transform.hpp"

namespace gir
{
	class SolutionCandidate
	{
	private:
		unsigned int m_Fitness = 0;
		unsigned int m_LinesSize;
		std::vector<std::pair<sf::Vector2f, sf::Vector2f>>* m_LinesPtr;
		std::vector<std::pair<sf::Vector2f, sf::Vector2f>> m_TransformedLines;
		std::vector<sf::Vector2f> m_Translations;
		std::vector<float> m_Rotations;
		Mat<Uint8> m_Solution;
		
		std::shared_ptr<RNG> m_Rng;
	public:
		SolutionCandidate();
		SolutionCandidate(std::vector<std::pair<sf::Vector2f, sf::Vector2f>>* lines, const Mat<Uint8>& threshEdges, std::shared_ptr<RNG> rng);
		SolutionCandidate(const SolutionCandidate& other);
		SolutionCandidate(SolutionCandidate&& other);
		~SolutionCandidate();

		SolutionCandidate& operator=(SolutionCandidate&& other);

		void ComputeFitness(const Mat<Uint8>& threshEdges);
		void ComputeSolution();
		static void Crossover(const SolutionCandidate& parent1, const SolutionCandidate& parent2, SolutionCandidate& child1, SolutionCandidate& child2);
		void Mutate(double transMutChance, double rotMutChance);
		
		inline unsigned int GetFitness() const { return m_Fitness; }
		inline bool operator< (const SolutionCandidate& other) const { return m_Fitness < other.m_Fitness; }
		inline bool operator> (const SolutionCandidate& other) const { return m_Fitness > other.m_Fitness; }
		inline bool operator<= (const SolutionCandidate& other) const { return m_Fitness <= other.m_Fitness; }
		inline bool operator>= (const SolutionCandidate& other) const { return m_Fitness >= other.m_Fitness; }

		
	private:
		void ClampLine(std::pair<sf::Vector2f, sf::Vector2f>& line);
		void BresenhamsLine(const std::pair<sf::Vector2f, sf::Vector2f>& line);	
	};
}
