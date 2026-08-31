
#include "MedicalStation.h"



MedicalStation::MedicalStation(const std::string& name, int capacity, int staffOnDuty) : EventUnit(name, capacity) {

    this->staffOnDuty = staffOnDuty;
}

void MedicalStation::reportStatus() const{

    printCommonStatus();
    std::cout << ", Staff on Duty: " << staffOnDuty << "\n";
}



void MedicalStation::update(const Notice& notice){

    EventUnit::update(notice);

    if(notice.getType() == NoticeType::EVACUATE){

        staffOnDuty += 2;
    }

}
