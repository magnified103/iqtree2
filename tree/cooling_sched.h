//
// Created by Dung Nguyen on 07/04/2025.
//

#ifndef IQTREE_COOLING_SCHED_H
#define IQTREE_COOLING_SCHED_H

#include <memory>
#include "json.hpp"

using json = nlohmann::json;

namespace sa {
class CoolingSchedule {
public:
    virtual void increaseIterCount() = 0;
    virtual void reheat() = 0;
    virtual double getTemperature() const = 0;
};

std::unique_ptr<CoolingSchedule> createCoolingSchedule(json config);
}

#endif //IQTREE_COOLING_SCHED_H
