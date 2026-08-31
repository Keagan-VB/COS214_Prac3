#include "Shuttle.h"

Shuttle::Shuttle(const std::string& name, int capacity, std::string route) : EventUnit(name, capacity) {

    this->route = route;
}

void Shuttle::reportStatus() const{

    printCommonStatus();
    std::cout << ", Route: " << route << "\n";
}

void Shuttle::update(const Notice& notice){

    EventUnit::update(notice);

    if(notice.getType() == NoticeType::EVACUATE || notice.getType() == NoticeType::WEATHER_ALERT){

        route = "Evacuation Loop";
    }

}