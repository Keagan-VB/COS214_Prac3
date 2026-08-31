#ifndef SHUTTLE_H
#define SHUTTLE_H

#include "EventUnit.h"


class Shuttle : public EventUnit{

    private:
    std::string route;

    public:
    Shuttle(const std::string& name, int capacity, std::string route);
    void reportStatus() const override;
    void update(const Notice& notice) override;

};

#endif