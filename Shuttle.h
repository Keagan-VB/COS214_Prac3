#ifndef SHUTTLE_H
#define SHUTTLE_H
#include "EventUnit.h"

/** @brief Concrete Leaf representing a shuttle service, tracking its current route. */
class Shuttle : public EventUnit {
private:
    /** @brief The current route the shuttle is servicing. */
    std::string route;

public:
    /** @brief Constructs a Shuttle with a name, capacity, and default route. */
    Shuttle(const std::string& name, int capacity, std::string route);

    /** @brief Prints the shuttle's status including its current route. */
    void reportStatus() const override;

    /** @brief Reacts to notices, specifically changing route during weather or evacuation alerts. */
    void update(const Notice& notice) override;
};
#endif