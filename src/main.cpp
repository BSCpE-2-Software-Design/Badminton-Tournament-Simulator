#include "include/Player.h"
#include "include/Enemy.h"
#include "include/Match.h"
#include "include/Schedule.h"
#include "include/SimulationEngine.h"
#include "include/StandingTracker.h"
#include "include/Display.h"
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    Display::displayWelcome();
    
    SimulationEngine engine;
    StandingTracker tracker;
    
    bool running = true;
    
    while (running) {
        Display::displayMainMenu();
        int choice;
        std::cin >> choice;
        
        switch (choice) {
            case 1: {
                // Start tournament
                Display::clearScreen();
                Display::displayModeSelection();
                int modeChoice;
                std::cin >> modeChoice;
                
                Match::MatchType mode;
                int playerNeeded;
                
                if (modeChoice == 1) {
                    mode = Match::SINGLES;
                    playerNeeded = 1;
                } else if (modeChoice == 2) {
                    mode = Match::DOUBLES;
                    playerNeeded = 2;
                } else {
                    Display::displayError("Invalid choice!");
                    Display::pressEnter();
                    break;
                }
                
                // Get all available characters
                std::vector<Player> allPlayers = Enemy::getAllCharacters();
                
                // Player selection
                std::vector<Player> selectedPlayers;
                
                for (int i = 0; i < playerNeeded; i++) {
                    Display::clearScreen();
                    Display::displayPlayerSelection(allPlayers, i + 1);
                    
                    int selectedId;
                    bool validSelection = false;
                    
                    while (!validSelection) {
                        std::cout << "Enter player ID for player " << (i + 1) << ": ";
                        std::cin >> selectedId;
                        
                        // Check if player already selected
                        bool alreadySelected = false;
                        for (const auto& p : selectedPlayers) {
                            if (p.getId() == selectedId) {
                                alreadySelected = true;
                                Display::displayError("Player already selected!");
                                break;
                            }
                        }
                        
                        if (!alreadySelected && selectedId >= 0 && selectedId < Enemy::TOTAL_CHARACTERS) {
                            selectedPlayers.push_back(Enemy::getCharacterById(selectedId));
                            validSelection = true;
                        } else if (!alreadySelected) {
                            Display::displayError("Invalid player ID!");
                        }
                    }
                }
                
                // Create tournament with selected players + computer players
                std::vector<Player> tournamentPlayers = selectedPlayers;
                
                // Add remaining players to tournament
                for (const auto& allPlayer : allPlayers) {
                    bool alreadyAdded = false;
                    for (const auto& selected : selectedPlayers) {
                        if (allPlayer.getId() == selected.getId()) {
                            alreadyAdded = true;
                            break;
                        }
                    }
                    if (!alreadyAdded) {
                        tournamentPlayers.push_back(allPlayer);
                    }
                }
                
                // Initialize tracker with all players
                for (const auto& player : tournamentPlayers) {
                    tracker.addPlayer(player);
                }
                
                // Generate schedule
                Schedule schedule(mode);
                if (mode == Match::SINGLES) {
                    schedule.generateRoundRobinSingles(tournamentPlayers);
                } else {
                    schedule.generateRoundRobinDoubles(tournamentPlayers);
                }
                
                // Attach tracker to engine
                engine.attach(&tracker);
                
                // Run tournament
                Display::clearScreen();
                schedule.displaySchedule();
                Display::pressEnter();
                
                std::vector<Match> matches = schedule.getMatches();
                
                Display::clearScreen();
                std::cout << "Starting tournament simulation..." << std::endl;
                
                for (size_t i = 0; i < matches.size(); i++) {
                    Display::displayMatchProgress(i + 1, matches.size());
                    engine.simulateMatch(matches[i]);
                    Display::displayMatchResult(matches[i]);
                }
                
                // Display final standings
                Display::clearScreen();
                tracker.printLeaderboard();
                tracker.printDetailed();
                Display::pressEnter();
                
                break;
            }
            
            case 2: {
                // View available players
                Display::clearScreen();
                Enemy::displayAllCharacters();
                Display::pressEnter();
                break;
            }
            
            case 3: {
                // Exit
                std::cout << "\nThank you for playing! Goodbye!" << std::endl;
                running = false;
                break;
            }
            
            default:
                Display::displayError("Invalid choice! Please try again.");
                Display::pressEnter();
        }
    }
    
    return 0;
}
