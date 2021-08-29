#pragma once
#include "ImgOps.h"
#include "RNG.h"
#include "SFML/Graphics/Transform.hpp"

namespace gir {

class SolutionCandidate {
private:
    int m_Fitness = 0;
    std::size_t m_LinesSize;
    std::vector<Line>* m_LinesPtr;
    std::vector<sf::Vector2f> m_Translations;
    std::vector<double> m_Rotations;

    std::shared_ptr<RNG> m_Rng;

public:
    SolutionCandidate(std::vector<Line>* lines, const Mat<Uint8>& threshEdges, const std::shared_ptr<RNG>& rng);
    SolutionCandidate(std::vector<Line>* lines,
                      std::size_t solutionRows,
                      std::size_t solutionCols,
                      const std::shared_ptr<RNG>& rng);

    void ComputeFitness(const Mat<Uint8>& threshEdges);
    static void Crossover(const SolutionCandidate& parent1,
                          const SolutionCandidate& parent2,
                          SolutionCandidate& child1,
                          SolutionCandidate& child2);
    void Mutate(double transMutChance, double rotMutChance);

    inline int GetFitness() const { return m_Fitness; }
    inline bool operator<(const SolutionCandidate& other) const { return m_Fitness < other.m_Fitness; }
    inline bool operator>(const SolutionCandidate& other) const { return m_Fitness > other.m_Fitness; }
    inline bool operator<=(const SolutionCandidate& other) const { return m_Fitness <= other.m_Fitness; }
    inline bool operator>=(const SolutionCandidate& other) const { return m_Fitness >= other.m_Fitness; }

    std::vector<Line> TransformedLines() const;

private:
    void ClampLine(Line& line, std::size_t rows, std::size_t columns) const;
    bool WithinBounds(const Line& line, std::size_t rows, std::size_t columns) const;
};

}   // namespace gir
