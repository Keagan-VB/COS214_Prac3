#ifndef EVENTLOG_H
#define EVENTLOG_H

#include <string>
#include <vector>

/**
 * @brief A lightweight run history for the demo/simulation.
 *
 * EventLog has exactly one job: remember short text entries in the
 * order they happened, and print them back out on request. It knows
 * nothing about Notice, EventComponent, EventControl, or any other
 * EventFlow class - EventScheduler is the only thing that writes to
 * it, and it writes plain strings. That single, narrow responsibility
 * is deliberate: it is what keeps this an original feature (Task 4.4)
 * rather than a second place where event-domain logic could grow.
 */
class EventLog {
public:
    /** @brief Constructs an empty log. */
    EventLog() = default;

    /**
     * @brief Appends an entry to the log.
     * @param entry Short description of what happened. Stored as-is.
     */
    void record(const std::string& entry);

    /**
     * @brief Returns how many entries have been recorded so far.
     * @return The number of recorded entries.
     */
    int size() const;

    /**
     * @brief Returns true if no entries have been recorded yet.
     * @return true if the log is empty.
     */
    bool empty() const;

    /**
     * @brief Prints every recorded entry, in order, to std::cout.
     * @param title Heading printed above the log entries.
     */
    void print(const std::string& title = "Event Log") const;

private:
    std::vector<std::string> entries;
};

#endif // EVENTLOG_H
