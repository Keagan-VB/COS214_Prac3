#ifndef MEDICALSTATION_H
#define MEDICALSTATION_H

#include "EventUnit.h"


class MedicalStation : public EventUnit{

    private:
    int staffOnDuty;

    public:
    MedicalStation(const std::string& name, int capacity, int staffOnDuty);
    void reportStatus() const override;
    void update(const Notice& notice) override;

};

#endif