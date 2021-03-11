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
        std::size_t m_Iteration = 0;
        std::size_t m_PopSize = 0;
        std::size_t m_Elitismn = 0;
        double m_TransMutChance = 0;
        double m_RotMutChance = 0;

        std::vector<SolutionCandidate> m_Population;

        Mat<Uint8> m_ThreshEdges;
        std::vector<Line> m_Lines;
        std::shared_ptr<RNG> m_Rng;

        std::size_t m_ImgRows = 0;
        std::size_t m_ImgCols = 0;
    public:
        GeneticOptimizer() = delete;
        GeneticOptimizer(std::size_t popSize, double transMutChance,
                         double rotMutChance, std::size_t elitismn);
    public:
        void PrepareGA(const sf::Image& origImage, Uint8 threshold,
                       std::size_t minLineLen, std::size_t maxLineLen);
        const SolutionCandidate& RunIterations(std::size_t nIterations);
        std::string GetInfo() const;

        inline const std::vector<Line>& Lines() const { return m_Lines; }
        inline std::size_t LinesSize() const { return m_Lines.size(); }
        inline const Mat<Uint8>& ThreshEdges() const { return m_ThreshEdges; }
    private:
        SelectionPair Selection(const std::vector<double>& weights);
    };
}

