
#include "TicketDesk.h"



TicketDesk::TicketDesk(const std::string& name, int capacity, bool admitting) : EventUnit(name, capacity) {

    this->admitting = admitting;
}

void TicketDesk::reportStatus() const{

    printCommonStatus();
    std::cout << ", Admitting:" << admitting << "\n";
}

void TicketDesk::close(){

    EventUnit::close();

    admitting = false;
}

void TicketDesk::open(){

    EventUnit::open();

    admitting = true;
}


void TicketDesk::update(const Notice& notice){

    EventUnit::update(notice);

    if(notice.getType() == NoticeType::CAPACITY_ALERT){

        admitting = false;
    } else if(notice.getType() == NoticeType::RESUME){

        admitting = true;
    }

}
