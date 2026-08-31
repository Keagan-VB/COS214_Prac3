#ifndef SUBJECT_H
#define SUBJECT_H

class Observer;

/**
 * @brief Abstract Subject interface for Observer registration.
 */
class Subject {
public:
    /**
     * @brief Registers an observer for future notifications.
     * @param observer Observer to register. Must not be nullptr.
     */
    virtual void attach(Observer* observer) = 0;

    /**
     * @brief Deregisters a previously attached observer.
     * @param observer Observer to remove.
     */
    virtual void detach(Observer* observer) = 0;

    /**
     * @brief Notifies every currently registered observer.
     */
    virtual void notify() = 0;

    /**
     * @brief Virtual destructor.
     */
    virtual ~Subject() = default;
};

#endif