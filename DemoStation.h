
#include "EventUnit.h"


class DemoStation : public EventUnit{

    private:
    int stationsAvailable;

    public:
    DemoStation(const std::string& name, int capacity, int stationsAvailable);
    void reportStatus() const override;
    void close() override;
    void update(const Notice& notice) override;
    int getCapacity() const;

};