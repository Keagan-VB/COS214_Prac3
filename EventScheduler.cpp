#include "EventScheduler.h"
#include "EventLog.h"
#include <iostream>

EventScheduler::EventScheduler(EventLog* log) : cursor(0), log(log) {}

void EventScheduler::addStep(const std::string& label, std::function<void()> action) {
    steps.push_back(Step{label, action});
}

void EventScheduler::scheduleNotice(EventControl& control, NoticeType type, const std::string& message) {
    addStep("issueNotice: " + message, [&control, type, message]() {
        control.issueNotice(type, message);
    });
}

void EventScheduler::scheduleOpen(EventComponent* component) {
    if (component == nullptr) return;
    addStep("open: " + component->getName(), [component]() {
        component->open();
    });
}

void EventScheduler::scheduleClose(EventComponent* component) {
    if (component == nullptr) return;
    addStep("close: " + component->getName(), [component]() {
        component->close();
    });
}

bool EventScheduler::runNext() {
    if (cursor >= static_cast<int>(steps.size())) {
        return false;
    }

    const Step& step = steps[cursor];
    std::cout << "\n[Scheduler " << (cursor + 1) << "/" << steps.size() << "] " << step.label << "\n";

    if (step.action) {
        step.action();
    }

    if (log != nullptr) {
        log->record(step.label);
    }

    ++cursor;
    return true;
}

void EventScheduler::runAll() {
    while (runNext()) {
        // keep going until the schedule is finished
    }
}

int EventScheduler::size() const {
    return static_cast<int>(steps.size());
}

int EventScheduler::position() const {
    return cursor;
}

bool EventScheduler::isFinished() const {
    return cursor >= static_cast<int>(steps.size());
}
