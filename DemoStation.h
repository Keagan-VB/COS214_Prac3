#ifndef DEMOSTATION_H
#define DEMOSTATION_H
#include "EventUnit.h"
#include <iostream>

/** @brief Concrete Leaf representing a gaming demo station, tracking available units. */
class DemoStation : public EventUnit {
private:
    /** @brief The number of demo stations currently available. */
    int stationsAvailable;

public:
    /**
     * @brief Constructs a DemoStation with a name, capacity, and available stations.
     * @param name The station's display name.
     * @param capacity The station's fixed capacity.
     * @param stationsAvailable The number of demo stations available at construction.
     */
    DemoStation(const std::string& name, int capacity, int stationsAvailable);

    /** @brief Prints the station's status including available units. */
    void reportStatus() const override;

    /** @brief Closes the station and resets available units to zero. */
    void close() override;

    /**
     * @brief Reacts to notices, reduces stations on capacity alerts.
     * @param notice The incoming notice.
     */
    void update(const Notice& notice) override;
};
#endif