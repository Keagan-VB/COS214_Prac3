#ifndef EVENTGROUP_H
#define EVENTGROUP_H

#include "EventComponent.h"
#include "Subject.h"
#include "Observer.h"

#include <vector>
#include <iostream>



class EventGroup : public EventComponent, public Subject, public Observer{

    private:
    std::vector<EventComponent*> children;
    std::vector<Observer*> observerList;
    const Notice* currentNotice;

    public:
    EventGroup(const std::string& name, int capacityThreshold);
    void add(EventComponent* c);
    void remove(EventComponent* c);
    void open() override;
    void close() override;
    void reportStatus() const override;
    int getCapacity() const override;
    void attach(Observer* o) override;
    void detach(Observer* o) override;
    void notify() override;
    void update(const Notice& n) override;

    bool transferChild(EventComponent* child, EventGroup* destination);

    ~EventGroup() override;

    EventGroup(const EventGroup&) = delete;
    EventGroup& operator=(const EventGroup&) = delete;



};

#endif