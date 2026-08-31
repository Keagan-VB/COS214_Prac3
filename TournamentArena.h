#ifndef TOURNAMENTARENA_H
#define TOURNAMENTARENA_H

#include "EventUnit.h"


class TournamentArena : public EventUnit{

    private:
    std::string currentMatch;

    public:
    TournamentArena(const std::string& name, int capacity, std::string currentMatch);
    void reportStatus() const override;
    void update(const Notice& notice) override;

};

#endif