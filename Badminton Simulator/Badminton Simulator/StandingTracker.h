#pragma once
#include <vector>
#include "IMatchObserver.h"

class Player;
class Match;

class StandingTracker : public IMatchObserver {
private:
    std::vector<Player*> _players;

public:
    StandingTracker();
    
    // Add player to tracking
    void AddPlayer(Player* player);
    
    // Observer implementation
    void OnMatchCompleted(Match* match) override;
    
    // Display leaderboard sorted by wins, then by total points
    void PrintLeaderboard() const;
};
