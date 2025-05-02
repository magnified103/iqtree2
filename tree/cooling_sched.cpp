//
// Created by Dung Nguyen on 07/04/2025.
//


#include <cmath>
#include "json.hpp"
#include "cooling_sched.h"

using json = nlohmann::json;

namespace sa {
class BoundedCoolingSchedule : public CoolingSchedule {
    // bounded by the number of iterations
    // reheats occur when the number of iterations reaches max_iter
protected:
    const double temp_start;
    const double temp_end;
    const int max_iter;
    int current_iter;
    int current_cooling_iter;
    const int cooling_cycle;  // number of iterations between each cooling
public:
    BoundedCoolingSchedule(double temp_start, double temp_end, int max_iter, int cooling_cycle = 1)
            : temp_start(temp_start), temp_end(temp_end), max_iter(max_iter), current_iter(0),
              current_cooling_iter(0), cooling_cycle(cooling_cycle) {}

    void increaseIterCount() override {
        current_iter++;
        if (current_iter >= max_iter) {
            // reheat
            reheat();
        }
        if (current_iter % cooling_cycle == cooling_cycle - 1) {
            // decrease temp
            current_cooling_iter = current_iter;
        }
    }

    void reheat() override {
        current_iter = 0;
        current_cooling_iter = 0;
    }
};
class BoundedExponentialCoolingSchedule : public BoundedCoolingSchedule {
    using BoundedCoolingSchedule::BoundedCoolingSchedule;
    // t_n = t_{n-1} * \alpha
    double getTemperature() const override {
        return this->temp_start *
                std::pow(this->temp_end / this->temp_start, (double) this->current_cooling_iter / this->max_iter);
    }
};
class BoundedLinearCoolingSchedule : public BoundedCoolingSchedule {
    using BoundedCoolingSchedule::BoundedCoolingSchedule;
    // t_n = t_{n-1} - \alpha
    double getTemperature() const override {
        return this->temp_start - (this->temp_start - this->temp_end) * ((double) this->current_cooling_iter / this->max_iter);
    }
};

class UnboundedCoolingSchedule : public CoolingSchedule {
    // not bounded by the number of iterations
    // reheats must be triggered manually
protected:
    const double temp_start;
    const double temp_end;
    const int max_iter;
    int current_iter;
    int current_cooling_iter;
    const int cooling_cycle;  // number of iterations between each cooling
public:
    UnboundedCoolingSchedule(double temp_start, double temp_end, int max_iter, int cooling_cycle = 1)
            : temp_start(temp_start), temp_end(temp_end), max_iter(max_iter), current_iter(0),
              current_cooling_iter(0), cooling_cycle(cooling_cycle) {}

    void increaseIterCount() override {
        current_iter++;
        if (current_iter % cooling_cycle == cooling_cycle - 1) {
            // decrease temp
            current_cooling_iter = current_iter;
        }
    }

    void reheat() override {
        current_iter = 0;
        current_cooling_iter = 0;
    }
};

class UnboundedExponentialCoolingSchedule : public UnboundedCoolingSchedule {
    using UnboundedCoolingSchedule::UnboundedCoolingSchedule;
    // t_n = t_{n-1} * \alpha
    double getTemperature() const override {
        return std::max(this->temp_end, this->temp_start *
                std::pow(this->temp_end / this->temp_start, (double) this->current_cooling_iter / this->max_iter));
    }
};

class UnboundedLinearCoolingSchedule : public UnboundedCoolingSchedule {
    using UnboundedCoolingSchedule::UnboundedCoolingSchedule;
    // t_n = t_{n-1} - \alpha
    double getTemperature() const override {
        return std::max(this->temp_end, this->temp_start - (this->temp_start - this->temp_end) * ((double) this->current_cooling_iter / this->max_iter));
    }
};

std::unique_ptr<CoolingSchedule> createCoolingSchedule(json config) {
    std::string type = config["type"];
    double temp_start = config["temp_start"].get<double>();
    double temp_end = config["temp_end"].get<double>();
    int max_iter = config["max_iter"].get<int>();
    int cooling_cycle = config.contains("cooling_cycle") ? config["cooling_cycle"].get<int>() : 1;
    if (type == "bounded_exponential") {
        return std::make_unique<BoundedExponentialCoolingSchedule>(temp_start, temp_end, max_iter, cooling_cycle);
    } else if (type == "bounded_linear") {
        return std::make_unique<BoundedLinearCoolingSchedule>(temp_start, temp_end, max_iter, cooling_cycle);
    } else if (type == "unbounded_exponential") {
        return std::make_unique<UnboundedExponentialCoolingSchedule>(temp_start, temp_end, max_iter, cooling_cycle);
    } else if (type == "unbounded_linear") {
        return std::make_unique<UnboundedLinearCoolingSchedule>(temp_start, temp_end, max_iter, cooling_cycle);
    }
    throw std::invalid_argument("Unknown cooling schedule type: " + type);
}
} // namespace sa
