#include <cstdlib>
#include <chrono>
#include <ctime>
#include <random>

#include "Casino/player.h"


#define _SPEED_X 0.5f
#define _SPEED_Y 0.5f

#define _MEAN 750
#define _STD 400

Player::Player(int index, float x_spawn, float y_spawn, float x_slot, float y_slot, float x_exit, float y_exit) :
	last_time{ std::chrono::high_resolution_clock::now() },
	slot{index},
	x_target_slot{x_slot},
	y_target_slot{y_slot},
	x_target_exit{x_exit},
	y_target_exit{y_exit},
	x_spawn_m{x_spawn},
	y_spawn_m{y_spawn},
	x_stop{false},
	y_stop{false},
	player_state{TOSLOT}
{
	// Create a random device
    std::random_device rd;
    
    // Initialize a Mersenne Twister random number generator
    std::mt19937 gen(rd());
    
    // Define a normal distribution with mean 0 and standard deviation 1
    std::normal_distribution<> dis(_MEAN, _STD);
    
    // Generate a random number
    this->money = dis(gen);
}

int Player::getSlot() {
	return this->slot;
}

float Player::getXToSlot() {
	if (this->x_stop) {
		return x_target_slot;
	}

	auto now = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - this->last_time).count();

	float duration_float = static_cast<float>(duration);
	float increment = duration_float * _SPEED_X;

	float currentX = x_spawn_m + increment;

	if (currentX > this->x_target_slot) {
		this->x_stop = true;
		this->last_time = std::chrono::high_resolution_clock::now();

		return x_target_slot;
	}

	return currentX;
}

float Player::getYToSlot() {
	
	if (!this->x_stop) return y_spawn_m;

	auto now = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - this->last_time).count();

	float duration_float = static_cast<float>(duration);
	float increment = duration_float * _SPEED_Y;

	float currentY = y_spawn_m - increment;

	if (currentY < this->y_target_slot) {
		this->player_state = ONSLOT;

		return this->y_target_slot;
	}

	return currentY;
}

float Player::getXToExit() {
	if (!this->y_stop) return this->x_target_slot;

	auto now = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - this->last_time).count();

	float duration_float = static_cast<float>(duration);
	float increment = duration_float * _SPEED_Y;

	float currentX = x_target_slot + increment;

	if (currentX > this->x_target_exit) {
		this->player_state = OUT;

		return this->y_target_exit;
	}

	return currentX;
}

float Player::getYToExit() {
	if (this->y_stop) {
		return y_target_exit;
	}

	auto now = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - this->last_time).count();

	float duration_float = static_cast<float>(duration);
	float increment = duration_float * _SPEED_X;

	float currentY = this->y_target_slot + increment;

	if (currentY > this->y_target_exit) {
		this->y_stop = true;
		this->last_time = std::chrono::high_resolution_clock::now();

		return y_target_exit;
	}

	return currentY;
}

PlayerState Player::getState() {
	return this->player_state;
}

void Player::back() {
	this->player_state = ONSLOT;
}

void Player::exit() {
	this->player_state = EXIT;
	
	this->x_stop = true;
	this->y_stop = false;

	this->last_time = std::chrono::high_resolution_clock::now();
}

bool Player::nextGame() {
	player_count++;

	if (player_count < 3) {
		return true;
	}

	return false;
}

void Player::play(Casino_Manager &casino_manager) {
	this->player_state = PLAYING;
	this->last_result = casino_manager.random_win_probability();

	last_time = std::chrono::high_resolution_clock::now();
}

void Player::check() {
	auto now = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - this->last_time).count();

	if (duration > 2000) {
		this->player_state = RESULT;
	}
}

bool Player::lastResult() {
	return this->last_result;
}