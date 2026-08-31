#ifndef EVENTSCHEDULER_H
#define EVENTSCHEDULER_H

#include "EventComponent.h"
#include "EventControl.h"
#include <functional>
#include <string>
#include <vector>

class EventLog;

/**
 * @brief A queue of scripted run-of-show steps, stepped through one at
 * a time.
 *
 * EventScheduler turns "issue this notice, then trigger that
 * reorganisation, then change this registration" from a wall of
 * sequential calls in main() into a reusable, replayable script. Each
 * Step is just a label plus a callable action; EventScheduler does not
 * know or care what the action actually does - it only knows how to
 * hold steps in order, run them one at a time (or all at once), and
 * optionally hand each step's label to an EventLog as it fires.
 *
 * Deliberately, this header only depends on EventComponent.h and
 * EventControl.h - the two interfaces the whole team froze on day one
 * - so it is buildable and unit-testable with stub components/EventControl
 * before EventGroup, EventUnit, or any concrete leaf exists. Anything
 * that needs a more specific type (e.g. EventGroup::transferChild for a
 * runtime reorganisation) is supplied from the call site as a generic
 * addStep() action; EventScheduler itself never needs to know EventGroup
 * exists.
 */
class EventScheduler {
public:
    /**
     * @brief A single scripted step: a human-readable label plus the
     * action to run when the step executes.
     */
    struct Step {
        std::string label;
        std::function<void()> action;
    };

    /**
     * @brief Constructs an empty scheduler.
     * @param log Optional EventLog that each step's label is recorded
     * to as it runs. Pass nullptr to run without logging. Not owned.
     */
    explicit EventScheduler(EventLog* log = nullptr);

    /**
     * @brief Appends a generic scripted step.
     * @param label Human-readable description shown/logged when the
     * step runs.
     * @param action The operation to perform when this step executes.
     */
    void addStep(const std::string& label, std::function<void()> action);

    /**
     * @brief Convenience: schedules an EventControl::issueNotice() call.
     * @param control The EventControl to issue the notice through.
     * @param type The notice category.
     * @param message The notice message.
     */
    void scheduleNotice(EventControl& control, NoticeType type, const std::string& message);

    /**
     * @brief Convenience: schedules opening a component via the
     * common EventComponent interface.
     * @param component The component to open. Ignored if nullptr.
     */
    void scheduleOpen(EventComponent* component);

    /**
     * @brief Convenience: schedules closing a component via the
     * common EventComponent interface.
     * @param component The component to close. Ignored if nullptr.
     */
    void scheduleClose(EventComponent* component);

    /**
     * @brief Runs the next unexecuted step in the queue, if any.
     * Prints the step's position and label before running its action,
     * and records the label to the EventLog (if one was supplied).
     * @return true if a step was run, false if the schedule is finished.
     */
    bool runNext();

    /** @brief Runs every remaining step, in order, until the schedule is finished. */
    void runAll();

    /**
     * @brief Returns the total number of steps ever scheduled.
     * @return The step count.
     */
    int size() const;

    /**
     * @brief Returns how many steps have already run.
     * @return The current cursor position.
     */
    int position() const;

    /**
     * @brief Returns true once every scheduled step has been run.
     * @return true if there are no more steps to run.
     */
    bool isFinished() const;

private:
    std::vector<Step> steps;
    int cursor;
    EventLog* log; ///< Non-owning; may be nullptr.
};

#endif // EVENTSCHEDULER_H
