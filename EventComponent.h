#ifndef EVENTCOMPONENT_H
#define EVENTCOMPONENT_H

#include <string>

/** @brief Abstract Component interface for Composite pattern, 
 * defining the common structure and operations for all event parts. 
 */
class EventComponent {
public:
    /**
     * @brief Opens this component, marking it active.
     */
    virtual void open() = 0;

    /**
     * @brief Closes this component, marking it inactive.
     */
    virtual void close() = 0;

    /**
     * @brief Prints a status report for this component.
     * A Leaf reports its own state. A Composite report every one 
     * of its children, recursively
     */
    virtual void reportStatus() const = 0;

    /**
     * @brief Returns this component's capacity.
     *
     * A Leaf returns its own fixed capacity. A Composite returns
     * the live sum of all its children's capacities/
     *
     * @return The current capacity.
     */
    virtual int getCapacity() const = 0;

    /**
     * @brief Returns this component's name.
     *
     * Not virtual: every derived class wants 
     * the exact same behaviour here
     *
     * @return The component's name.
     */
    std::string getName() const;

    /**
     * @brief Virtual destructor for safe polymorphic destruction.
     *
     */
    virtual ~EventComponent() = default;

protected:
    /**
     * @brief Constructs the shared Component state.
     *
     * Protected: EventComponent is abstract and must only ever be
     * constructed as part of building a derived class.
     *
     * @param name The component's name.
     * @param capacity The component's capacity.
     */
    EventComponent(const std::string& name, int capacity);

    std::string name;
    int capacity;
};

#endif