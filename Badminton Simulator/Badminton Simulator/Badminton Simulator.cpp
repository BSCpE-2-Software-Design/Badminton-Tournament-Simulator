#include <iostream>
#include <vector>
#include "Player.h"
#include "Match.h"
#include "TournamentScheduler.h"
#include "BadmintonSimulationEngine.h"
#include "StandingTracker.h"

int main() {
    std::cout << "========================================\n";
    std::cout << "  BADMINTON TOURNAMENT SIMULATOR\n";
    std::cout << "  Round-Robin Tournament\n";
    std::cout << "========================================\n\n";

    // Step 1: Create players with Stamina and Skill Level
    std::cout << "[STEP 1] Creating Players...\n";
    std::vector<Player*> players;
    players.push_back(new Player("jamaica", 15, 18));   // High stamina & skill
    players.push_back(new Player("jao", 20, 15));     // Medium stats
    players.push_back(new Player("princess", 18, 12)); // High stamina, med skill
    players.push_back(new Player("johngie", 16, 16));   // Balanced high stats

    std::cout << "Players created:\n";
    for (auto player : players) {
        std::cout << "  - " << player->getName() << " (Stamina: "
            << player->getStamina() << ", Skill: " << player->getSkillLevel() << ")\n";
    }
    std::cout << std::endl;

    // Step 1.5: Allow user to choose a player to track
    std::cout << "[STEP 1.5] Select a Player to Track\n";
    std::cout << "Available players:\n";
    for (size_t i = 0; i < players.size(); i++) {
        std::cout << "  " << (i + 1) << ". " << players[i]->getName() << "\n";
    }
    
    int playerChoice = 0;
    Player* trackedPlayer = nullptr;
    while (playerChoice < 1 || playerChoice > (int)players.size()) {
        std::cout << "Enter player number (1-" << players.size() << "): ";
        std::cin >> playerChoice;
        
        if (playerChoice < 1 || playerChoice > (int)players.size()) {
            std::cout << "Invalid choice. Please enter a number between 1 and " << players.size() << ".\n";
        }
    }
    trackedPlayer = players[playerChoice - 1];
    std::cout << "You have selected: " << trackedPlayer->getName() << "\n\n";

    // Step 2: Generate Round-Robin Schedule
    std::cout << "[STEP 2] Generating Round-Robin Schedule...\n";
    std::vector<Match> schedule = TournamentScheduler::CreateRoundRobin(players);
    std::cout << "Total matches: " << schedule.size() << "\n";
    std::cout << "Matchups:\n";
    for (size_t i = 0; i < schedule.size(); i++) {
        std::cout << "  Match " << (i + 1) << ": " << schedule[i].getSideA()->getName()
            << " vs " << schedule[i].getSideB()->getName() << "\n";
    }
    std::cout << std::endl;

    // Step 3: Initialize Simulation Engine and Observer
    std::cout << "[STEP 3] Initializing Simulation Engine...\n";
    BadmintonSimulationEngine engine;
    StandingTracker tracker;

    // Attach tracker as observer to engine
    engine.Attach(&tracker);

    // Add all players to tracker
    for (auto player : players) {
        tracker.AddPlayer(player);
    }
    std::cout << "Engine and tracker initialized.\n";
    std::cout << "Tracking player: " << trackedPlayer->getName() << "\n\n";

    // Step 4: Simulate all matches
    std::cout << "[STEP 4] Simulating Matches...\n";
    std::cout << "===========================\n\n";
    for (size_t i = 0; i < schedule.size(); i++) {
        std::cout << "Simulating Match " << (i + 1) << "...\n";
        
        // Highlight if tracked player is in this match
        if (schedule[i].getSideA() == trackedPlayer || schedule[i].getSideB() == trackedPlayer) {
            std::cout << "*** " << trackedPlayer->getName() << " is in this match! ***\n";
        }
        
        engine.RunMatch(&schedule[i]);
        
        // Show tracked player stats after each match
        std::cout << "Tracked Player Stats: " << trackedPlayer->getName() 
            << " - Stamina: " << trackedPlayer->getStamina() 
            << ", Skill: " << trackedPlayer->getSkillLevel() << "\n";
        std::cout << std::endl;
    }

    // Step 5: Display Final Standings
    std::cout << "\n[STEP 5] Tournament Complete!\n";
    tracker.PrintLeaderboard();

    // Step 6: Display final stats for tracked player
    std::cout << "\n[STEP 6] Final Stats for Selected Player: " << trackedPlayer->getName() << "\n";
    std::cout << "  Stamina: " << trackedPlayer->getStamina() << "\n";
    std::cout << "  Skill Level: " << trackedPlayer->getSkillLevel() << "\n";

    // Cleanup
    for (auto player : players) {
        delete player;
    }

    std::cout << "\nThank you for using Badminton Tournament Simulator!\n";
    return 0;
}
