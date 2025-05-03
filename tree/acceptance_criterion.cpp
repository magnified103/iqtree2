#include "acceptance_criterion.h"
#include <cmath>
#include <random>

namespace sa {
class BoltzmannAcceptanceCriterion : public AcceptanceCriterion {
private:
    std::mt19937 rnd;
    std::uniform_real_distribution<double> dist;
public:
    BoltzmannAcceptanceCriterion(int seed): rnd(seed), dist() {}
    bool accept(double delta, double temperature) override {
        if (delta > 0) {
            return true;
        }
        return std::exp(delta / temperature) > dist(rnd);
    }
};

class AdaptiveBoltzmannAcceptanceCriterion : public AcceptanceCriterion {
private:
    std::mt19937 rnd;
    std::uniform_real_distribution<double> dist;
public:
    AdaptiveBoltzmannAcceptanceCriterion(int seed): rnd(seed), dist(), AcceptanceCriterion() {}
    bool accept(double delta, double temperature) override {
        if (delta > 0) {
            return true;
        }
        double averageDelta = bestScore - sumOfScores / numOfScores;
        return std::exp(delta / averageDelta / temperature) > dist(rnd);
    }
};

std::unique_ptr<AcceptanceCriterion> createAcceptanceCriterion(json config) {
    std::string type = config["type"];
    int seed = config["seed"];
    if (type == "adaptive_boltzmann") {
        return std::make_unique<AdaptiveBoltzmannAcceptanceCriterion>(seed);
    } else if (type == "boltzmann") {
        return std::make_unique<BoltzmannAcceptanceCriterion>(seed);
    } else {
        throw std::invalid_argument("Unknown acceptance criterion type: " + config["type"].get<std::string>());
    }
}
}
