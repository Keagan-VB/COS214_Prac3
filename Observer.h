#ifndef OBSERVER_H
#define OBSERVER_H

class Notice;

/**
 * @brief Abstract Observer interface for receiving Notices.
 */
class Observer {
public:
    /**
     * @brief Pushes a Notice to this Observer.
     * @param notice The delivered Notice. Reference is only valid during the call.
     */
    virtual void update(const Notice& notice) = 0;

    /**
     * @brief Virtual destructor.
     */
    virtual ~Observer() = default;
};

#endif