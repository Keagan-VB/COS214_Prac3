#ifndef TICKETDESK_H
#define TICKETDESK_H

#include "EventUnit.h"


class TicketDesk : public EventUnit{

    private:
    bool admitting;

    public:
    TicketDesk(const std::string& name, int capacity, bool admitting);
    void reportStatus() const override;
    void close() override;
    void open() override;
    void update(const Notice& notice) override;

};

#endif