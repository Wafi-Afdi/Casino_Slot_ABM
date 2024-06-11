#include <thread>
#include <iostream>
#include <cstdlib>
#include <mutex>
#include "Game_Manager/gui.h"

#define _HORIZONTAL_WINDOW 800
#define _VERTICAL_WINDOW 600

#define _VERTICAL_SLOT 100.0f
#define _PLAYER_STOP_INCREMENT 50.0f

std::mutex mu;

Gui::Gui(const char* c) : sf::RenderWindow(sf::VideoMode(_HORIZONTAL_WINDOW, _VERTICAL_WINDOW), c)
{
    this->setup();
}

void Gui::setup() {
    this->font.loadFromFile("BrownieStencil-8O8MJ.ttf");
    this->slotTexture.loadFromFile("slot-machine-3.png");

    // Set the desired ratio
    float desiredWidth = 400.0f;
    float desiredHeight = 300.0f;

    // Calculate the scaling factors
    float scaleX = desiredWidth / slotTexture.getSize().x;
    float scaleY = desiredHeight / slotTexture.getSize().y;
    
    
    float sideLength = 50.0f;

    sf::Vector2f position[4];

    position[0] = sf::Vector2f(150.0f, _VERTICAL_SLOT); 
    position[1] = sf::Vector2f(300.0f, _VERTICAL_SLOT); 
    position[2] = sf::Vector2f(450.0f, _VERTICAL_SLOT); 
    position[3] = sf::Vector2f(600.0f, _VERTICAL_SLOT);


    for (int i = 0; i < 4; i++) {
        this->square[i] = sf::RectangleShape(sf::Vector2f(sideLength, sideLength));
        this->square[i].setPosition(position[i]);
        this->square[i].setFillColor(sf::Color::Red);

        this->slotSprite[i].setTexture(slotTexture);
        this->slotSprite[i].setScale(sf::Vector2f(scaleX, scaleY));
        this->slotSprite[i].setPosition(position[i]);
    }
}

void Gui::drawSlot() {
    for (int i = 0; i < 4; i++) {
        this->draw(this->slotSprite[i]);
        this->draw(this->square[i]);
    }
}

void Gui::drawPlayer() {
    for (auto& player_object : this->po_vector) {
        this->draw(player_object.circle);
    }
}

void Gui::drawText() {
    for (auto& text : this->text_vector) {
        this->draw(*text);
    }
}

float Gui::getTargetX(int index) {
    switch (index) {
    case 0:
        return 150.0f + 5.0f;
    case 1:
        return 300.0f + 5.0f;
    case 2:
        return 450.0f + 5.0f;
    case 3:
        return 600.0f + 5.0f;
    }

    return -1;
}

float Gui::getTargetY() {
    return _VERTICAL_SLOT + _PLAYER_STOP_INCREMENT;
}

float Gui::getXExit() {
    return _HORIZONTAL_WINDOW;
}

float Gui::getYExit() {
    return _VERTICAL_WINDOW - 100;
}

void Gui::result(std::vector<PlayerObject>::iterator& it) {
    bool result = it->player->lastResult();
    this->resultLabel(result, it->player->getSlot());

    bool play = it->player->nextGame();
    if (play) {
        it->player->back();
    }
    else {
        it->player->exit();
        this->slot[it->player->getSlot()] = false;
    }
}

void Gui::resultLabel(bool win, int index) {
    if (win) {
        std::thread t(&Gui::winLabel, this, index);
        t.detach();
    }
    else {
        std::thread t(&Gui::loseLabel, this, index);
        t.detach();
    }
}

void Gui::winLabel(int index) {
    sf::Text* text = new sf::Text;
    text->setFont(this->font);
    text->setString("+100");
    text->setCharacterSize(24);
    text->setFillColor(sf::Color::Green);

    this->text_vector.push_back(text);

    float x = Gui::getTargetX(index);
    float y = Gui::getTargetY();

    auto last = std::chrono::high_resolution_clock::now();
    auto now = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count();
    float duration_float = static_cast<float>(duration);

    while (duration_float < 1000) {

        now = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count();
        duration_float = static_cast<float>(duration);

        float currentY = y - duration_float * 0.2f;

        text->setPosition(x, currentY);
    }

    mu.lock();
    auto it = std::find(this->text_vector.begin(), this->text_vector.end(), text);
    if (it != this->text_vector.end()) {
        this->text_vector.erase(it);
    }
    mu.unlock();

}

void Gui::loseLabel(int index) {
    sf::Text* text = new sf::Text;

    text->setFont(this->font);
    text->setString("-100");
    text->setCharacterSize(24);
    text->setFillColor(sf::Color::Red);

    this->text_vector.push_back(text);

    float x = Gui::getTargetX(index);
    float y = Gui::getTargetY();

    auto last = std::chrono::high_resolution_clock::now();
    auto now = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count();
    float duration_float = static_cast<float>(duration);

    while (duration_float < 1000) {

        now = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count();
        duration_float = static_cast<float>(duration);

        float currentY = y - duration_float * 0.2f;

        text->setPosition(x, currentY);
    }

    auto it = std::find(this->text_vector.begin(), this->text_vector.end(), text);
    if (it != this->text_vector.end()) {
        this->text_vector.erase(it);
    }
}