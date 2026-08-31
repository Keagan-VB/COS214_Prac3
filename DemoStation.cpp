
#include "DemoStation.h"



DemoStation::DemoStation(const std::string& name, int capacity, int stationsAvailable) : EventUnit(name, capacity) {

    this->stationsAvailable = stationsAvailable;
}

void DemoStation::reportStatus() const{

    printCommonStatus();
    std::cout << ", Stations Available:" << stationsAvailable << "\n";
}

void DemoStation::close(){

    EventUnit::close();

    stationsAvailable = 0;
}


void DemoStation::update(const Notice& notice){

    EventUnit::update(notice);

    if(notice.getType() == NoticeType::CAPACITY_ALERT){

        if(stationsAvailable < 1){

            stationsAvailable = 0;
        }else{

            stationsAvailable -= 1;
        }
    }

}
