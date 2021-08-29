#include "GeneticOptimizer.h"

namespace gir {
GeneticOptimizer::GeneticOptimizer(std::size_t popSize,
                                   double transMutChance,
                                   double rotMutChance,
                                   std::size_t elitismn)
    : m_PopSize(popSize)
    , m_Elitismn(elitismn)
    , m_TransMutChance(transMutChance)
    , m_RotMutChance(rotMutChance) {
    assert((m_PopSize - m_Elitismn) % 2 == 0);
    m_Population.reserve(m_PopSize);
}

void GeneticOptimizer::PrepareGA(const sf::Image& origImage,
                                 Uint8 threshold,
                                 std::size_t minLineLen,
                                 std::size_t maxLineLen) {
    assert(maxLineLen > minLineLen);

    const auto size = origImage.getSize();
    m_ImgRows = size.y;
    m_ImgCols = size.x;
    Mat<Uint8> gray(m_ImgRows, m_ImgCols);
    m_ThreshEdges.Resize(m_ImgRows, m_ImgCols);

    ToGrayscale(origImage, gray);
    Sobel(gray, m_ThreshEdges);
    Threshold(m_ThreshEdges, threshold);

    std::mt19937 gen((std::random_device())());
    std::uniform_int_distribution<std::size_t> lineLenDistr(minLineLen, maxLineLen);

    int whitePixelCount = m_ThreshEdges.ValueCount(255) * 1.5;
    assert(whitePixelCount > 0);

    while (whitePixelCount > 0) {
        // All lines are zero centered with y0=y1=0

        const int lineLen = lineLenDistr(gen);
        const int halfLineLen = lineLen / 2;

        whitePixelCount -= lineLen;
        if (whitePixelCount - lineLen < 0)
            break;

        m_Lines.emplace_back(sf::Vector2f(-halfLineLen, 0.0f), sf::Vector2f(halfLineLen, 0.0f));
    }

    m_Rng = std::make_shared<RNG>(m_ThreshEdges.Rows(), m_ThreshEdges.Cols(), m_Lines.size(), m_Lines.size() * 0.8);

    for (std::size_t i = 0; i < m_PopSize; i++) {
        m_Population.emplace_back(&m_Lines, m_ThreshEdges, m_Rng);
    }
}

static bool SelectionCompare(std::pair<double, std::size_t> l, std::pair<double, std::size_t> r) {
    return l.first > r.first;
}

GeneticOptimizer::SelectionPair GeneticOptimizer::Selection(const std::vector<double>& weights) {
    // Poor poor poor poor poor poor version of weighted selection
    std::size_t i1, i2;
    std::vector<std::pair<double, int>> weightedIndices;
    weightedIndices.reserve(weights.size());

    for (std::size_t i = 0; i < weights.size(); i++) {
        weightedIndices.emplace_back(m_Rng->Probability() * weights[i], i);
    }

    std::sort(weightedIndices.begin(), weightedIndices.end(), SelectionCompare);

    i1 = weightedIndices[0].second;
    i2 = weightedIndices[1].second;

    return std::make_pair(std::cref(m_Population[i1]), std::cref(m_Population[i2]));
}

const SolutionCandidate& GeneticOptimizer::RunIterations(std::size_t nIterations) {
    std::vector<double> weights(m_PopSize);

    for (std::size_t i = 0; i < nIterations; i++) {
        double accFitness = 0.0;
        std::vector<SolutionCandidate> newPopulation;

        // Copy the best solutions from the last iteration
        newPopulation.reserve(m_PopSize);
        for (std::size_t i = 0; i < m_Elitismn; i++) {
            newPopulation.push_back(m_Population[i]);
        }

        // Calculate the probability distribution
        for (const auto& sc : m_Population) {
            accFitness += sc.GetFitness();
        }

        if (accFitness == 0.0)
            accFitness = 1.0;

        for (std::size_t i = 0; i < m_PopSize; i++) {
            weights[i] = m_Population[i].GetFitness() / accFitness;
        }

        while (newPopulation.size() < m_PopSize) {
            // Perform the selection
            const auto& [parent1, parent2] = Selection(weights);

            // Crossover
            SolutionCandidate child1(&m_Lines, m_ImgRows, m_ImgCols, m_Rng);
            SolutionCandidate child2(&m_Lines, m_ImgRows, m_ImgCols, m_Rng);
            SolutionCandidate::Crossover(parent1, parent2, child1, child2);

            // Mutate the children [TODO: maybe add annealing ?]
            child1.Mutate(m_TransMutChance, m_RotMutChance);
            child2.Mutate(m_TransMutChance, m_RotMutChance);

            // Calculate the fitness of the new units
            child1.ComputeFitness(m_ThreshEdges);
            child2.ComputeFitness(m_ThreshEdges);

            // Add the children to the new population
            newPopulation.push_back(std::move(child1));
            newPopulation.push_back(std::move(child2));
        }

        m_Iteration++;
        m_Population.swap(newPopulation);
        std::sort(m_Population.begin(), m_Population.end(), std::greater<SolutionCandidate>());
    }

    return m_Population[0];
}


std::string GeneticOptimizer::GetInfo() const {
    std::stringstream ss;
    ss << "Iteration: " << m_Iteration << "\n";

    //for (std::size_t i = 0; i < m_PopSize; i++)
    //	ss << "Fitness " << i << " :" << m_Population[i].GetFitness() << "\n";

    // Population is always sorted before this function is called.
    ss << "Top fitness: " << m_Population[0].GetFitness() << "\n";

    return ss.str();
}
}   // namespace gir