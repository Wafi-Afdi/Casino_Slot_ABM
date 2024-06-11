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
	player_state{TOSLOT},
	agent_stop{false},
	out_of_money{false}
{
	// Create a random device
	std::random_device rd;
	// Initialize a Mersenne Twister random number generator
	std::mt19937 gen(rd());
	std::normal_distribution<> norm_dis(_MEAN, _STD);

	// Generate a random number
	this->money = (int)norm_dis(gen);
	//printf("MONEY : %i\n", this->money);

	std::uniform_real_distribution<> uni_dis(0.0, 0.2);
	this->probability_stop = (float)uni_dis(gen);
	//printf("PROB STOP : %f\n", this->probability_stop);

	std::uniform_real_distribution<> uni_dis2(-0.05, 0.05);
	this->win_increment_probability = (float)uni_dis2(gen);
	this->loss_increment_probability = (float)uni_dis2(gen);
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

bool Player::nextGame(Casino_Manager& casino_manager) {
	if (this->money - casino_manager.getSlotPrice() < 0) {
		out_of_money = true;
		return false;
	}
	bool canPlay = this->canPlayAgain();

	return canPlay;
}

void Player::play(Casino_Manager& casino_manager) {
	this->player_state = PLAYING;
	
	casino_manager.add_game();
	this->money -= casino_manager.getSlotPrice();
	casino_manager.add_revenue(casino_manager.getSlotPrice());

	this->last_result =	casino_manager.random_win_probability();

	if (this->last_result) {
		this->probability_stop += win_increment_probability;
		if (this->probability_stop > 1) {
			this->probability_stop = 1;
		}
		else if (this->probability_stop < 0) {
			this->probability_stop = 0;
		}

		// nambahin duit agen dan nambah loss di kasino
		this->money += casino_manager.getSlotCashout();
		casino_manager.add_loss(casino_manager.getSlotCashout());
	}
	else {
		this->probability_stop += loss_increment_probability;
		if (this->probability_stop > 1) {
			this->probability_stop = 1;
		}
		else if (this->probability_stop < 0) {
			this->probability_stop = 0;
		}
	}

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

bool Player::canPlayAgain() {
	// Create a random device
	std::random_device rd;
	// Initialize a Mersenne Twister random number generator
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> uni_dis(0.0, 1.0);
	float random_number = (float)uni_dis(gen);
	if (random_number < this->probability_stop) {
		agent_stop = true;
		return false;
	}
	else {
		return true;
	}
}