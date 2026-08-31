#ifndef EVENTUNIT_H
#define EVENTUNIT_H

#include "Observer.h"
#include "EventComponent.h"
#include "Notice.h"
#include "NoticeType.h"

#include <iostream>


class EventUnit : public EventComponent, public Observer{

    protected:
    bool operational;
    bool maintenanceMode;
    void printCommonStatus() const;

    public:
    EventUnit(const std::string& name, int capacity);
    void open() override;
    void close() override;
    int getCapacity() const override;
    void update(const Notice& notice) override;
    void setMaintenanceMode(bool inMaintenance);

    virtual ~EventUnit() = default;



};

#endif