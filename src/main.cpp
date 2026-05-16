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
#include <cstdlib>
#include <ctime>

int main() {
    srand(static_cast<unsigned>(time(0)));
    
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
                std::vector<int> selectedIds;
                
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
                        for (int id : selectedIds) {
                            if (id == selectedId) {
                                alreadySelected = true;
                                Display::displayError("Player already selected!");
                                break;
                            }
                        }
                        
                        if (!alreadySelected && selectedId >= 0 && selectedId < Enemy::TOTAL_CHARACTERS) {
                            selectedPlayers.push_back(Enemy::getCharacterById(selectedId));
                            selectedIds.push_back(selectedId);
                            validSelection = true;
                        } else if (!alreadySelected) {
                            Display::displayError("Invalid player ID!");
                        }
                    }
                }
                
                // Display selected team
                Display::clearScreen();
                Display::displayTeamSelection(selectedPlayers, playerNeeded);
                Display::pressEnter();
                
                // Create list of enemies (all players except selected ones)
                std::vector<Player> enemies;
                for (const auto& allPlayer : allPlayers) {
                    bool isSelected = false;
                    for (int selectedId : selectedIds) {
                        if (allPlayer.getId() == selectedId) {
                            isSelected = true;
                            break;
                        }
                    }
                    if (!isSelected) {
                        enemies.push_back(allPlayer);
                    }
                }
                
                // Create matches: Your team vs 5 random enemies
                std::vector<Match> matches;
                int matchId = 1;
                
                // We'll play against 5 different opponents
                int numMatches = std::min(5, (int)enemies.size());
                
                // Shuffle enemies for random selection
                std::vector<int> enemyIndices;
                for (size_t i = 0; i < enemies.size(); i++) {
                    enemyIndices.push_back(i);
                }
                
                for (int i = 0; i < 100; i++) {
                    int r1 = rand() % enemies.size();
                    int r2 = rand() % enemies.size();
                    std::swap(enemyIndices[r1], enemyIndices[r2]);
                }
                
                // Generate matches against 5 random enemies
                for (int m = 0; m < numMatches; m++) {
                    if (mode == Match::SINGLES) {
                        // 1v1 match
                        std::vector<Player> yourTeam = {selectedPlayers[0]};
                        std::vector<Player> enemyTeam = {enemies[enemyIndices[m]]};
                        
                        Match match(matchId++, Match::SINGLES, yourTeam, enemyTeam);
                        matches.push_back(match);
                    } else {
                        // 2v2 match - need 2 different enemies
                        std::vector<Player> yourTeam = selectedPlayers;
                        std::vector<Player> enemyTeam;
                        
                        enemyTeam.push_back(enemies[enemyIndices[m]]);
                        
                        // Find another enemy for 2v2
                        int secondEnemyIdx = (m + 1) % enemies.size();\n                        if (secondEnemyIdx == m) secondEnemyIdx = (secondEnemyIdx + 1) % enemies.size();
                        \n                        enemyTeam.push_back(enemies[enemyIndices[secondEnemyIdx]]);
                        
                        Match match(matchId++, Match::DOUBLES, yourTeam, enemyTeam);
                        matches.push_back(match);
                    }
                }
                
                // Initialize tracker with selected players
                for (const auto& player : selectedPlayers) {
                    tracker.addPlayer(player);
                }
                
                // Attach tracker to engine
                engine.attach(&tracker);
                
                // Display matches schedule
                Display::clearScreen();
                std::cout << "\n" << std::string(80, '=') << std::endl;
                std::string typeStr = (mode == Match::SINGLES) ? \"SINGLES (1v1)\" : \"DOUBLES (2v2)\";
                std::cout << \"Tournament Mode: \" << typeStr << std::endl;
                std::cout << \"Your Team: \";\n                for (const auto& p : selectedPlayers) {\n                    std::cout << p.getName() << \" \";\n                }\n                std::cout << std::endl;\n                std::cout << \"Total Matches: \" << matches.size() << std::endl;\n                std::cout << std::string(80, '=') << std::endl;\n                \n                std::cout << \"\\nMatches:\" << std::endl;\n                for (size_t i = 0; i < matches.size(); i++) {\n                    std::cout << \"Match \" << (i + 1) << \": \";\n                    for (const auto& p : matches[i].getTeamA()) {\n                        std::cout << p.getName() << \" \";\n                    }\n                    std::cout << \"vs \";\n                    for (const auto& p : matches[i].getTeamB()) {\n                        std::cout << p.getName() << \" \";\n                    }\n                    std::cout << std::endl;\n                }\n                std::cout << std::string(80, '=') << std::endl;\n                Display::pressEnter();\n                \n                // Run tournament\n                Display::clearScreen();\n                std::cout << \"Starting tournament simulation...\" << std::endl << std::endl;\n                \n                int wins = 0;\n                int losses = 0;\n                \n                for (size_t i = 0; i < matches.size(); i++) {\n                    Display::displayMatchProgress(i + 1, matches.size());\n                    engine.simulateMatch(matches[i]);\n                    Display::displayMatchResult(matches[i]);\n                    \n                    // Track wins/losses for your team\n                    if (matches[i].getScoreA() > matches[i].getScoreB()) {\n                        wins++;\n                    } else {\n                        losses++;\n                    }\n                }\n                \n                // Display final standings (only your players)\n                Display::clearScreen();\n                std::cout << \"\\n\" << std::string(80, '=') << std::endl;\n                std::cout << \"TOURNAMENT COMPLETE!\" << std::endl;\n                std::cout << std::string(80, '=') << std::endl;\n                \n                std::cout << \"\\nYour Team Statistics:\" << std::endl;\n                std::cout << std::string(80, '-') << std::endl;\n                for (const auto& player : selectedPlayers) {\n                    auto stats = tracker.getPlayerStats(player.getId());\n                    std::cout << std::left\n                              << std::setw(20) << stats.getName()\n                              << std::setw(10) << (\"Wins: \" + std::to_string(stats.getWins()))\n                              << std::setw(10) << (\"Losses: \" + std::to_string(stats.getLosses()))\n                              << std::setw(12) << (\"Points: \" + std::to_string(stats.getTotalPoints())) << std::endl;\n                }\n                std::cout << std::string(80, '=') << std::endl;\n                \n                std::cout << \"\\nTeam Overall Record: \" << wins << \" wins, \" << losses << \" losses\" << std::endl;\n                if (wins > losses) {\n                    std::cout << \"🏆 CONGRATULATIONS! Your team is the champion! 🏆\" << std::endl;\n                } else if (wins == losses) {\n                    std::cout << \"⚖️  Balanced performance! Keep improving!\" << std::endl;\n                } else {\n                    std::cout << \"💪 Better luck next time! Keep training!\" << std::endl;\n                }\n                std::cout << std::string(80, '=') << std::endl;\n                Display::pressEnter();\n                \n                // Reset tracker for next tournament\n                tracker = StandingTracker();\n                \n                break;\n            }\n            \n            case 2: {\n                // View available players\n                Display::clearScreen();\n                Enemy::displayAllCharacters();\n                Display::pressEnter();\n                break;\n            }\n            \n            case 3: {\n                // Exit\n                std::cout << \"\\nThank you for playing! Goodbye!\" << std::endl;\n                running = false;\n                break;\n            }\n            \n            default:\n                Display::displayError(\"Invalid choice! Please try again.\");\n                Display::pressEnter();\n        }\n    }\n    \n    return 0;\n}
