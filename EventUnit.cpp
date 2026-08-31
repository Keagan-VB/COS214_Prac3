#include "EventUnit.h"



EventUnit::EventUnit(const std::string& name, int capacity) : EventComponent(name, capacity){

    operational = true;
    maintenanceMode = false;
}


void EventUnit::open(){

    operational = true;
}

void EventUnit::close(){

    operational = false;
}

int EventUnit::getCapacity() const{

    if(maintenanceMode){

        return 0;
    }else{

        return capacity;
    }

}

void EventUnit::update(const Notice& notice){

    if(maintenanceMode && notice.getType() != NoticeType::EVACUATE){
 
        return;
    }
    if(notice.getType() == NoticeType::OPEN){
        open();
    }else if(notice.getType() == NoticeType::CLOSE){
        close();
    }
}

void EventUnit::setMaintenanceMode(bool inMaintenance){

    maintenanceMode = inMaintenance;
}

void EventUnit::printCommonStatus() const{

    std::cout << name << " (" << operational << ", "<<maintenanceMode << ")";
}
