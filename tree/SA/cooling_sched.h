//
// Created by Dung Nguyen on 07/04/2025.
//

#ifndef IQTREE_COOLING_SCHED_H
#define IQTREE_COOLING_SCHED_H

namespace sa {
class CoolingSchedule {
    virtual void decreaseTemperature() = 0;
    virtual void reheat() = 0;
    virtual double getTemperature() const = 0;
};
}

#endif //IQTREE_COOLING_SCHED_H
