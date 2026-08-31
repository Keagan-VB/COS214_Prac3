#ifndef FOODVENDOR_H
#define FOODVENDOR_H

#include "EventUnit.h"


class FoodVendor : public EventUnit{

    private:
    int stockLevel;

    public:
    FoodVendor(const std::string& name, int capacity, int stockLevel);
    void reportStatus() const override;
    void update(const Notice& notice) override;

};

#endif