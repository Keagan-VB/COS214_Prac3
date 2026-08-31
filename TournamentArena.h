#ifndef TOURNAMENTARENA_H
#define TOURNAMENTARENA_H
#include "EventUnit.h"

/** @brief Concrete Leaf representing a tournament arena, tracking the current match. */
class TournamentArena : public EventUnit {
private:
    /** @brief The name of the match currently taking place. */
    std::string currentMatch;

public:
    /** @brief Constructs a TournamentArena with a name, capacity, and current match name. */
    TournamentArena(const std::string& name, int capacity, std::string currentMatch);

    /** @brief Prints the arena's status including the currently running match. */
    void reportStatus() const override;

    /** @brief Reacts to notices, specifically pausing/resuming operations for weather or schedule changes. */
    void update(const Notice& notice) override;
};
#endif