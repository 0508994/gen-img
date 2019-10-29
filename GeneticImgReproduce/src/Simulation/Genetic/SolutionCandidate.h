#pragma once
#include "ImgOps.h"
#include <random>
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
	public:
		static std::random_device s_Rd;								
		static std::mt19937 s_Generator;							
	public:
		SolutionCandidate();
		SolutionCandidate(std::vector<std::pair<sf::Vector2f, sf::Vector2f>>* lines, const Mat<Uint8>& threshEdges);
		~SolutionCandidate();

		void ComputeFitness(const Mat<Uint8>& threshEdges);
		void ComputeSolution();
		void Crossover(SolutionCandidate& other);
		void Mutate(double transMutChance, double rotMutChance);
		
		inline unsigned int GetFitness() const { return m_Fitness; }
		inline bool operator< (const SolutionCandidate& other) const { return m_Fitness < other.m_Fitness; }
		inline bool operator> (const SolutionCandidate& other) const { return m_Fitness > other.m_Fitness; }
		
	private:
		void ClampLine(std::pair<sf::Vector2f, sf::Vector2f>& line);
		void BresenhamsLine(const std::pair<sf::Vector2f, sf::Vector2f>& line);	
	};
}
