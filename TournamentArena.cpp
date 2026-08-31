#include "TournamentArena.h"

TournamentArena::TournamentArena(const std::string& name, int capacity, std::string currentMatch) : EventUnit(name, capacity) {

    this->currentMatch = currentMatch;
}

void TournamentArena::reportStatus() const{

    printCommonStatus();
    std::cout << ", Current Match: " << currentMatch << "\n";
}

void TournamentArena::update(const Notice& notice){

    EventUnit::update(notice);

    if(notice.getType() == NoticeType::WEATHER_ALERT || notice.getType() == NoticeType::PAUSE){

        operational = false;
    }else if(notice.getType() == NoticeType::RESUME){

        operational = true;
    }

}