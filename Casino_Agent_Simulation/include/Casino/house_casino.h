#pragma once

#include "SFML/Graphics.hpp"
#include <random>
#include <map>

#define _PROBABILITY_WIN_SLOT 0.001

class Casino_Manager {
private:
    int revenue;
    int loss;
    int total_player;
    int total_game;
    int slotPrice;
    int slotCashout;

public:
    Casino_Manager() {
        revenue = 0;
        loss = 0;
        total_player = 0;
        total_game = 0;
        slotPrice = 10;
        slotCashout = 100;
    }

    void add_revenue(int increment) {
        revenue += increment;
    }

    void add_loss(int increment) {
        loss += increment;
    }

    void add_player() {
        total_player += 1;
    }

    void add_game() {
        //printf("PROFIT : %i\n", revenue - loss);
        total_game += 1;
    }

    int get_profit() {
        return revenue - loss;
    }

    int get_revenue() {
        return this->revenue;
    }

    std::vector<int> LReel = { 1,1,3,4,5,2,2,3,4,2,2,5,3,5,3,4,2,1,2,5,4,3 };
    std::vector<int> CReel = { 3,1,2,2,3,5,4,4,2,2,3,2,1,5,4,3,3,2,1,5,4,1 };
    std::vector<int> RReel = { 2,3,5,4,4,3,1,1,2,2,3,5,4,3,2,1,2,1,5,1,3,4 };

    std::map<int, int> Winner = {
        {111,2}, {222,7}, {333, 10}, {331, 2}, {332, 2},
        {334, 3}, {335, 4}, {444, 15}, {441,5}, {442, 5},
        {443, 2}, {445, 6}, {123, 2}, {551, 2}, {555, 70}
    };

    int random_win_probability() {
        // Create a random device
        std::random_device rd;

        // Initialize a Mersenne Twister random number generator
        std::mt19937 gen(rd());

        // Define a uniform real distribution from 0 to 1
        std::uniform_real_distribution<> dis(0, ((float)LReel.size()) - 1);

        // Generate a random number
        int left_index = (int)dis(gen);
        int middle_index = (int)dis(gen);
        int right_index = (int)dis(gen);

        int real_pos = LReel[left_index] * 100 + CReel[middle_index] * 10 + RReel[right_index];
        loss += Winner[real_pos] * 10;
        printf("NUMBER REEL : %i\n", real_pos);
        return Winner[real_pos] * 10;
    }

    int getSlotCashout() {
        return this->slotCashout;
    }

    int getSlotPrice() {
        return this->slotPrice;
    }

    int get_loss() {
        return this->loss;
    }

    int get_total() {
        return this->total_game;
    }
};