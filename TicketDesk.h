#ifndef TICKETDESK_H
#define TICKETDESK_H
#include "EventUnit.h"

/** @brief Concrete Leaf representing a ticket desk, tracking if it is currently admitting attendees. */
class TicketDesk : public EventUnit {
private:
    /** @brief Tracks whether the desk is currently letting people in. */
    bool admitting;

public:
    /** @brief Constructs a TicketDesk with a name, capacity, and initial admitting state. */
    TicketDesk(const std::string& name, int capacity, bool admitting);

    /** @brief Opens the desk and resumes admitting attendees. */
    void open() override;

    /** @brief Closes the desk and stops admitting attendees. */
    void close() override;

    /** @brief Prints the desk's status including its current admitting state. */
    void reportStatus() const override;

    /** @brief Reacts to notices, specifically toggling admission on capacity alerts or resume notices. */
    void update(const Notice& notice) override;
};
#endif