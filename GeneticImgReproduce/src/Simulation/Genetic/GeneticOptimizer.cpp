#include "GeneticOptimizer.h"

namespace gir
{
	GeneticOptimizer::GeneticOptimizer() {}
	
	GeneticOptimizer::GeneticOptimizer(unsigned int popSize, float transMutChance, float rotMutChance, unsigned int minLineLen, unsigned int maxLineLen)
		:m_PopSize(popSize),
		m_TransMutChance(transMutChance),
		m_RotMutChance(rotMutChance),
		m_MinLineLen(minLineLen),
		m_MaxLineLen(maxLineLen)
	{
		m_Population.reserve(m_PopSize);
	}


	GeneticOptimizer::~GeneticOptimizer()
	{
	}
}