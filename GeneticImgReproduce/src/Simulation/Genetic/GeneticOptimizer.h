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
        using SelectionPair = std::pair<const SolutionCandidate&, const SolutionCandidate&>;
    private:
        unsigned int m_Iteration = 0;
        unsigned int m_PopSize = 0;
        unsigned int m_Elitismn = 0;
        double m_TransMutChance = 0;
        double m_RotMutChance = 0;

        std::vector<SolutionCandidate> m_Population;

        Mat<Uint8> m_ThreshEdges;
        std::vector<Line> m_Lines;
        std::shared_ptr<RNG> m_Rng;

        unsigned int m_ImgRows = 0;
        unsigned int m_ImgCols = 0;
    public:
        GeneticOptimizer() = delete;
        GeneticOptimizer(unsigned int popSize, double transMutChance,
                         double rotMutChance, unsigned int elitismn);
    public:
        void PrepareGA(const sf::Image& origImage, Uint8 threshold,
                       unsigned int minLineLen, unsigned int maxLineLen);
        const SolutionCandidate& RunIterations(unsigned int nIterations);
        std::string GetInfo() const;

        inline const std::vector<Line>& Lines() const { return m_Lines; }
        inline unsigned int LinesSize() const { return m_Lines.size(); }
        inline const Mat<Uint8>& ThreshEdges() const { return m_ThreshEdges; }
    private:
        SelectionPair Selection(const std::vector<double>& weights);
    };
}

