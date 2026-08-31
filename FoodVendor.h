#ifndef FOODVENDOR_H
#define FOODVENDOR_H
#include "EventUnit.h"

/** @brief Concrete Leaf representing a food vendor, tracking stock levels. */
class FoodVendor : public EventUnit {
private:
    /** @brief The current stock level of the vendor. */
    int stockLevel;

public:
    /** @brief Constructs a FoodVendor with a name, capacity, and initial stock. */
    FoodVendor(const std::string& name, int capacity, int stockLevel);

    /** @brief Prints the vendor's status including current stock level. */
    void reportStatus() const override;

    /** @brief Reacts to notices, reduces stock on capacity alerts. */
    void update(const Notice& notice) override;
};
#endif