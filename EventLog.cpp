#include "EventLog.h"
#include <iostream>

void EventLog::record(const std::string& entry) {
    entries.push_back(entry);
}

int EventLog::size() const {
    return static_cast<int>(entries.size());
}

bool EventLog::empty() const {
    return entries.empty();
}

void EventLog::print(const std::string& title) const {
    std::cout << "\n=== " << title << " (" << entries.size() << " entries) ===\n";
    if (entries.empty()) {
        std::cout << "  (nothing recorded)\n";
        return;
    }
    for (std::size_t i = 0; i < entries.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << entries[i] << "\n";
    }
}
