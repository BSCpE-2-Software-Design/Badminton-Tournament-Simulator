#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>

class Player {
private:
    int id;
    std::string name;
    int power;           // Strength attribute (1-100)
    int agility;         // Speed attribute (1-100)
    int stamina;         // Endurance attribute (1-100)
    int wins;
    int losses;
    int pointsScored;
    int pointsAgainst;

public:
    Player();
    Player(int id, const std::string& name, int power, int agility, int stamina);

    // Getters
    int getId() const;
    std::string getName() const;
    int getPower() const;
    int getAgility() const;
    int getStamina() const;
    int getWins() const;
    int getLosses() const;
    int getPointsScored() const;
    int getPointsAgainst() const;
    double getWinRate() const;

    // Setters for statistics
    void addWin(int points);
    void addLoss(int points);
    void resetStats();

    // Display
    void display() const;
};

#endif // PLAYER_H
