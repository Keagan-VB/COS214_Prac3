#ifndef EVENTCONTROL_H
#define EVENTCONTROL_H

#include "Subject.h"
#include "Notice.h"
#include <vector>
#include <string>

/**
 * @brief Concrete top-level Subject that issues event-wide notices.
 */
class EventControl : public Subject {
public:
    EventControl();
    ~EventControl() override;

    void attach(Observer* observer) override;
    void detach(Observer* observer) override;
    void revokeLastNotice();
    void notify() override;

    /**
     * @brief Creates a Notice and triggers a notification cascade.
     * @param type The category of the notice.
     * @param message Human-readable details.
     */
    void issueNotice(NoticeType type, const std::string& message);

private:
    std::vector<Observer*> observerList;
    Notice* currentNotice;
    NoticeType lastNoticeType; 
};

#endif // EVENTCONTROL_H