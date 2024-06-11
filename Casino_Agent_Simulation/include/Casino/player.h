#pragma once

#include <chrono>

#include "Casino/house_casino.h"

enum PlayerState {
    TOSLOT,
    ONSLOT,
    PLAYING,
    RESULT,
    EXIT,
    OUT
};

class Player {
private:
    bool last_result = false;

    bool x_stop;
    bool y_stop;

    float x_target_slot;
    float y_target_slot;

    float x_target_exit;
    float y_target_exit;

    float x_spawn_m;
    float y_spawn_m;

    float probability_stop;
    float loss_increment_probability;
    float win_increment_probability;
    
    int money;

    int slot;

    std::chrono::time_point<std::chrono::high_resolution_clock> last_time;

    PlayerState player_state;

    int player_count = 0;

public:
    Player(int index, float x_spawn, float y_spawn, float x_slot, float y_slot, float x_exit, float y_exit);

    void exit();
    void play(Casino_Manager &casino_manager);
    void back();
    void check();

    bool lastResult();
    bool nextGame();

    int getSlot();

    float getXToSlot();
    float getYToSlot();
    float getXToExit();
    float getYToExit();

    PlayerState getState();
};


