//
// Created by Dung Nguyen on 07/04/2025.
//


#include <cmath>

#include "cooling_sched.h"

namespace sa {
    class BoundedCoolingSchedule : public CoolingSchedule {
    protected:
        double temp_start;
        double temp_end;
        int max_iter;
        int current_iter;
    public:
        BoundedCoolingSchedule(double temp_start, double temp_end, int max_iter)
                : temp_start(temp_start), temp_end(temp_end), max_iter(max_iter), current_iter(0) {}
        void decreaseTemperature() override {
            if (current_iter < max_iter) {
                current_iter++;
            } else {
                // reheat
                current_iter = 0;
            }
        }

        void reheat() override {
            current_iter = 0;
        }
    };
    class BoundedExponentialCoolingSchedule : public BoundedCoolingSchedule {
        double getTemperature() const override {
            return this->temp_start *
                   std::pow(this->temp_end / this->temp_start, (double) this->current_iter / this->max_iter);
        }
    };
    class BoundedLinearCoolingSchedule : public BoundedCoolingSchedule {
        double getTemperature() const override {
            return this->temp_start - (this->temp_start - this->temp_end) * ((double) this->current_iter / this->max_iter);
        }
    };
    class BoundedLogarithmicCoolingSchedule : public BoundedCoolingSchedule {
        double getTemperature() const override {
            return this->temp_start - (this->temp_start - this->temp_end) * std::log((double) this->current_iter + 1) / std::log(this->max_iter + 1);
        }
    };
}
