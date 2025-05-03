#ifndef IQTREE_ACCEPTANCE_CRITERION_H
#define IQTREE_ACCEPTANCE_CRITERION_H

#include <memory>
#include <string>
#include "json.hpp"

using json = nlohmann::json;

namespace sa {
class AcceptanceCriterion {
public:
    double bestScore;
    double sumOfScores;
    int numOfScores;
    AcceptanceCriterion() : bestScore(-1e18), sumOfScores(0), numOfScores(0) {}
    virtual void updateScore(double score) {
        sumOfScores += score;
        numOfScores++;
        if (score > bestScore) {
            bestScore = score;
        }
    }
    virtual bool accept(double delta, double temperature) = 0;
};

std::unique_ptr<AcceptanceCriterion> createAcceptanceCriterion(json config);
}

#endif  // IQTREE_ACCEPTANCE_CRITERION_H