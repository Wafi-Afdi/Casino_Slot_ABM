#pragma once

#include "SFML/Graphics.hpp"
#include <random>

#define _PROBABILITY_WIN_SLOT 0.001

//class Slot {
//    private:
//        sf::Vector2f pos;
//        sf::Vector2f play_pos;
//
//        float win_probability;
//        
//        long int harga_main;
//        long int total_game;
//        long int prize;
//        long int total_player;
//        long int total_revenue;   
//
//        sf::Time game_time;
//        sf::Time time_left;
//
//
//        // Player* player_lagi_main;
//
//    public:
//        Slot() {
//
//        };  
//        Slot(float harga_main, float win_probability, sf::Time game_time) {
//            this->harga_main = harga_main;
//            this->win_probability = win_probability;
//            this->game_time = sf::seconds(0.0f);
//        }
//
//        void set_pos(sf::Vector2f pos) {
//            this->pos = pos;
//            this->play_pos = sf::Vector2f(pos.x, pos.y + 15);
//        }
//
//        void set_player() {
//
//        }
//
//
//
//
//};


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
        slotPrice = 50;
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

    bool random_win_probability() {
        // Create a random device
        std::random_device rd;

        // Initialize a Mersenne Twister random number generator
        std::mt19937 gen(rd());

        // Define a uniform real distribution from 0 to 1
        std::uniform_real_distribution<> dis(0.0, 1.0);

        // Generate a random number
        float random_number = (float)dis(gen);
        if (random_number < _PROBABILITY_WIN_SLOT) {
            return true;
        }
        else {
            return false;
        }
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