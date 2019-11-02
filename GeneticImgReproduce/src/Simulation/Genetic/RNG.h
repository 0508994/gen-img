#pragma once
#include <random>

/*
	Class used for generating all random numbers that are needed through the entire project.
*/

namespace gir
{
	class RNG
	{
	private:
		std::mt19937 m_Generator;

		std::uniform_real_distribution<double> m_DistrAngle;
		std::uniform_real_distribution<double> m_Prob;
		std::uniform_real_distribution<double> m_Distr0toRows;
		std::uniform_real_distribution<double> m_Distr0toCols;
		std::uniform_int_distribution<unsigned int> m_DistrLineSplitInd;
		std::uniform_int_distribution<unsigned int> m_DistrLineInd;
		std::uniform_int_distribution<unsigned int> m_DistrMutLin;
	public:
		RNG() {};
		
		RNG(unsigned int nRows, unsigned int nColumns, unsigned int nLines, unsigned int nMaxLinesToMutate)
			:m_Generator((std::random_device())()),
			m_DistrAngle(0.0, 360),
			m_Prob(0.0, 1.0),
			m_Distr0toCols(0.0, static_cast<double>(nColumns)),
			m_Distr0toRows(0.0, static_cast<double>(nRows)),
			m_DistrLineInd(0, nLines - 1),
			m_DistrLineSplitInd(nLines / 2, nLines - 1),
			m_DistrMutLin(0, nMaxLinesToMutate) {}

		~RNG() {}

		inline std::mt19937& Generator() { return m_Generator; }

		inline double RandomAngle() { return m_DistrAngle(m_Generator); }
		inline double Probability() { return m_Prob(m_Generator); }
		inline double RandomX() { return m_Distr0toCols(m_Generator); }
		inline double RandomY() { return m_Distr0toRows(m_Generator); }
		inline unsigned int RandomLineIndex() { return m_DistrLineInd(m_Generator); }
		inline unsigned int RandomSplitIndex() { return m_DistrLineSplitInd(m_Generator); }
		inline unsigned int RandomLineNumber() { return m_DistrMutLin(m_Generator); }
	};
}