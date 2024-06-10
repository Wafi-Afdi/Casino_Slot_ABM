#pragma once

#include "SFML/Graphics.hpp"

class Slot {
    private:
        sf::Vector2f pos;
        sf::Vector2f play_pos;

        float win_probability;
        
        long int harga_main;
        long int total_game;
        long int prize;
        long int total_player;
        long int total_revenue;   

        sf::Time game_time;
        sf::Time time_left;


        // Player* player_lagi_main;

    public:
        Slot() {

        };  
        Slot(float harga_main, float win_probability, sf::Time game_time) {
            this->harga_main = harga_main;
            this->win_probability = win_probability;
            this->game_time = sf::seconds(0.0f);
        }

        void set_pos(sf::Vector2f pos) {
            this->pos = pos;
            this->play_pos = sf::Vector2f(pos.x, pos.y + 15);
        }

        void set_player() {

        }




};


class Casino_Manager {
    private:
        long int profit;
        long int revenue;
        long int loss;
        long int total_player;
        long int total_game;

    public:
        Casino_Manager() {
            profit = 0;
            revenue = 0;
            loss = 0;
            total_player = 0;
            total_game = 0;
        }

        void add_revenue(long int increment) {
            revenue += increment;
        }

        void add_loss(long int increment) {
            loss += increment;
        }

        void add_player() {
            total_player += 1;
        }

        void add_game() {
            total_game += 1;
        }
    
};