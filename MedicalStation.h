#ifndef MEDICALSTATION_H
#define MEDICALSTATION_H
#include "EventUnit.h"

/** @brief Concrete Leaf representing a medical station, tracking the number of staff on duty. */
class MedicalStation : public EventUnit {
private:
    /** @brief The number of medical staff currently active. */
    int staffOnDuty;

public:
    /** @brief Constructs a MedicalStation with a name, capacity, and initial staff count. */
    MedicalStation(const std::string& name, int capacity, int staffOnDuty);

    /** @brief Prints the station's status including the current number of staff on duty. */
    void reportStatus() const override;

    /** @brief Reacts to notices, specifically increasing staff during evacuation alerts. */
    void update(const Notice& notice) override;
};
#endif