#include "GeneticOptimizer.h"

namespace gir
{
	GeneticOptimizer::GeneticOptimizer() {}

	GeneticOptimizer::~GeneticOptimizer() {}
	
	GeneticOptimizer::GeneticOptimizer(unsigned int popSize, double transMutChance, double rotMutChance, unsigned int minLineLen, unsigned int maxLineLen)
		:m_PopSize(popSize),
		m_TransMutChance(transMutChance),
		m_RotMutChance(rotMutChance),
		m_MinLineLen(minLineLen),
		m_MaxLineLen(maxLineLen)
	{
		m_Population.reserve(m_PopSize);
	}

	void GeneticOptimizer::PrepareGA(const sf::Image& origImage, Uint8 threshold)
	{
		auto size = origImage.getSize();
		Mat<Uint8> gray(size.y, size.x);
		m_ThreshEdges.Resize(size.y, size.x);

		ToGrayscale(origImage, gray);
		Sobel(gray, m_ThreshEdges);
		Threshold(m_ThreshEdges, threshold);

		// Form Lines
		
		// Generate population
	}

}