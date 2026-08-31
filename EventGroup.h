#ifndef EVENTGROUP_H
#define EVENTGROUP_H
#include "EventComponent.h"
#include "Subject.h"
#include "Observer.h"
#include <vector>
#include <iostream>

/** @brief The Composite node of the event tree, holding children and acting as both Subject and Observer. */
class EventGroup : public EventComponent, public Subject, public Observer {
private:
    std::vector<EventComponent*> children;
    std::vector<Observer*> observerList;
    const Notice* currentNotice;

public:
    /**
     * @brief Constructs the group with a name and a capacity threshold.
     * @param name The group's display name.
     * @param capacityThreshold The aggregate capacity at/above which this
     * group treats a CAPACITY_ALERT as significant enough to rebroadcast
     * (see update()).
     */
    EventGroup(const std::string& name, int capacityThreshold);

    /**
     * @brief Adds a child component to this group, taking strict ownership.
     *
     * Task 6.3: If a child is transferred from another EventGroup,
     * the previous owner MUST call remove() first to prevent double-deletion. 
     * This method does not automatically detach the child
     * from its previous Subject registrations; Composite ownership and
     * Observer registration are managed as separate concerns.
     *
     * @param c The component to add. Takes ownership. Ignored if nullptr.
     */
    void add(EventComponent* c);

    /**
     * @brief Removes a child component from this group, releasing ownership.
     * @param c The component to remove. Not deleted; caller/new owner
     * becomes responsible for it. Safe no-op if not present.
     */
    void remove(EventComponent* c);

    /** @brief Opens the group and recursively opens all children. */
    void open() override;

    /** @brief Closes the group and recursively closes all children. */
    void close() override;

    /** @brief Prints the group's status and recursively reports all children. */
    void reportStatus() const override;

    /**
     * @brief Returns the live aggregate sum of all children's capacities.
     * @return The total capacity.
     */
    int getCapacity() const override;

    /**
     * @brief Registers an observer to this group's notification list.
     * @param o Observer to register. Duplicate registration is ignored.
     */
    void attach(Observer* o) override;

    /**
     * @brief Removes an observer from this group's notification list.
     * @param o Observer to remove. Safe no-op if not registered.
     */
    void detach(Observer* o) override;

    /** @brief Pushes the current notice to all registered observers. */
    void notify() override;

    /**
     * @brief Receives a notice from a parent Subject and rebroadcasts it to children.
     *
     * For a CAPACITY_ALERT, this group only rebroadcasts to its own
     * observers if its live aggregate capacity has reached the group's
     * threshold; all other notice types are always rebroadcast.
     *
     * @param n The incoming notice. Only guaranteed valid for the duration
     * of this call and any notify() triggered synchronously from within it.
     */
    void update(const Notice& n) override;

    /**
     * @brief Transfers a child to a destination group, updating ownership
     * and, if the child is also an Observer, its registration with this group.
     * @param child The component currently owned by this group.
     * @param destination The group that should take over ownership.
     * @return true if the transfer happened, false if child/destination
     * were null or child was not found in this group.
     */
    bool transferChild(EventComponent* child, EventGroup* destination);

    /** @brief Virtual destructor, recursively deletes all owned children. */
    ~EventGroup() override;

    // Prevent copying to avoid double-deletion of owned children
    EventGroup(const EventGroup&) = delete;
    EventGroup& operator=(const EventGroup&) = delete;
};
#endif