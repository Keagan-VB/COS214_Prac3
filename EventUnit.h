#ifndef EVENTUNIT_H
#define EVENTUNIT_H
#include "Observer.h"
#include "EventComponent.h"
#include "Notice.h"
#include <iostream>

/** @brief Abstract Leaf base for the Composite pattern, implementing the Observer interface to react to event notices. */
class EventUnit : public EventComponent, public Observer {
protected:

    /** @brief Tracks if unit is currently active/operational. */
    bool operational;
    
    /** @brief Tracks if unit is under maintenance, OG feature. */
    bool maintenanceMode;
    
    /** @brief Protected helper to print shared status for concrete leaves. */
    void printCommonStatus() const;

public:
    /** @brief Constructs the unit with a name, capacity, and default operational state. */
    EventUnit(const std::string& name, int capacity);

    /** @brief Opens the unit, marking it operational. */
    void open() override;

    /** @brief Closes the unit, marking it inactive. */
    void close() override;

    /**
     * @brief Returns the capacity, returning 0 if in maintenance mode.
     * @return The current capacity.
     */
    int getCapacity() const override;

    /** @brief Reacts to notices, handling OPEN/CLOSE and maintenance gates. */
    void update(const Notice& notice) override;

    /** @brief Toggles the maintenance mode state for this unit. */
    void setMaintenanceMode(bool inMaintenance);

    /** @brief Virtual destructor for safe polymorphic destruction. */
    virtual ~EventUnit() = default;
};
#endif