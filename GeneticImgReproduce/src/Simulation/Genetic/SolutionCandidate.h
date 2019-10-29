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
		std::vector<std::pair<sf::Vector2f, sf::Vector2f>>* m_LinesPtr;
		std::vector<std::pair<sf::Vector2f, sf::Vector2f>> m_TransformedLines;
		std::vector<sf::Vector2f> m_Translations;
		std::vector<float> m_Rotations;
		Mat<Uint8> m_Solution;
	public:
		SolutionCandidate();
		SolutionCandidate(std::vector<std::pair<sf::Vector2f, sf::Vector2f>>* lines, const Mat<Uint8>& threshEdges);
		~SolutionCandidate();

		void ComputeFitness(const Mat<Uint8>& threshEdges);
		inline unsigned int GetFitness() const { return m_Fitness; }

		void ComputeSolution();
		
		inline bool operator< (const SolutionCandidate& other) const { return m_Fitness < other.m_Fitness; }
		inline bool operator> (const SolutionCandidate& other) const { return m_Fitness > other.m_Fitness; }
		
	private:
		void BresenhamsLine(const std::pair<sf::Vector2f, sf::Vector2f>& line);	
	};
}
