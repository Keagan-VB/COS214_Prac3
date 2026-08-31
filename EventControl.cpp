#include "EventControl.h"
#include "Observer.h"
#include <algorithm>

EventControl::EventControl() : currentNotice(nullptr) {}

EventControl::~EventControl() {
    if (currentNotice != nullptr) {
        delete currentNotice;
    }
}

void EventControl::attach(Observer* observer) {
    if (observer == nullptr) return;
    
    // Prevent duplicate registration
    auto it = std::find(observerList.begin(), observerList.end(), observer);
    if (it == observerList.end()) {
        observerList.push_back(observer);
    }
}

void EventControl::detach(Observer* observer) {
    // Safely remove if exists, do nothing if not registered
    observerList.erase(std::remove(observerList.begin(), observerList.end(), observer), observerList.end());
}

void EventControl::notify() {
    if (currentNotice == nullptr) return;
    
    for (Observer* obs : observerList) {
        obs->update(*currentNotice);
    }
}

void EventControl::issueNotice(NoticeType type, const std::string& message) {
    if (currentNotice != nullptr) {
        delete currentNotice;
    }
    
    // Hold as internal state before notifying
    currentNotice = new Notice(type, message);
    this->lastNoticeType = type;
    notify();
}

void EventControl::revokeLastNotice() {
    switch(lastNoticeType) {
        case NoticeType::PAUSE:
            issueNotice(NoticeType::RESUME, "Pause revoked. Resuming operations.");
            break;
        case NoticeType::CLOSE:
            issueNotice(NoticeType::OPEN, "Closure revoked. Area is now open.");
            break;
        case NoticeType::WEATHER_ALERT:
            issueNotice(NoticeType::RESUME, "Weather cleared. All clear.");
            break;
        default:
            // Do nothing if it's not a reversible notice
            break;
    }
}