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

#include <iomanip>
#include <sstream>

std::string formatTime(float time) {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << time;
    return stream.str();
}

Gui::Gui(const char* c) : sf::RenderWindow(sf::VideoMode(_HORIZONTAL_WINDOW, _VERTICAL_WINDOW), c)
{
    this->setup();
}

void Gui::setup() {
    this->font.loadFromFile("BrownieStencil-8O8MJ.ttf");
    this->slotTexture.loadFromFile("slot-machine-3.png");

    // Set the desired ratio
    float desiredWidth = 50;
    float desiredHeight = 50.0f;

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

    // setup legend
    legend_ui.text_broke.setFont(font);
    legend_ui.text_stop.setFont(font);

    legend_ui.text_broke.setFillColor(sf::Color::Black);
    legend_ui.text_stop.setFillColor(sf::Color::Black);

    legend_ui.text_broke.setCharacterSize(16);
    legend_ui.text_stop.setCharacterSize(16);

    legend_ui.text_broke.setString("Out of money");
    legend_ui.text_stop.setString("Choose To Stop");

    legend_ui.circle_broke.setRadius(20.0f);
    legend_ui.circle_stop.setRadius(20.0f);
    legend_ui.circle_stop.setFillColor(sf::Color::Magenta);
    legend_ui.circle_broke.setFillColor(sf::Color::Red);

    // setup position
    legend_ui.circle_stop.setPosition(sf::Vector2f(0, 0));
    legend_ui.circle_broke.setPosition(sf::Vector2f(0,50));


    sf::Vector2f text_post1 = sf::Vector2f(legend_ui.circle_stop.getPosition());
    text_post1.x += 50;
    text_post1.y += 10;
    legend_ui.text_stop.setPosition(text_post1);


    sf::Vector2f text_post2 = sf::Vector2f(legend_ui.circle_broke.getPosition());
    text_post2.x += 50;
    text_post2.y += 10;
    legend_ui.text_broke.setPosition(text_post2);

    this->setup_stats();

    

}

void Gui::drawSlot() {
    for (int i = 0; i < 4; i++) {
        //this->draw(this->square[i]);
        this->draw(this->slotSprite[i]);
    }
}

void Gui::drawPlayer() {
    for (auto& player_object : this->po_vector) {
        if (player_object.player->agent_stop) {
            player_object.circle.setFillColor(sf::Color::Magenta);
        }
        else if (player_object.player->out_of_money) {
            player_object.circle.setFillColor(sf::Color::Red);
        }
        else player_object.circle.setFillColor(sf::Color::Blue);
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

    bool play = it->player->nextGame(casino_manager);
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

void Gui::drawLegend() {
    this->draw(legend_ui.circle_broke);
    this->draw(legend_ui.circle_stop);
    this->draw(legend_ui.text_broke);
    this->draw(legend_ui.text_stop);
}

void Gui::setup_stats() {
    // setup legend
    stats_ui.text_loss.setFont(font);
    stats_ui.text_profit.setFont(font);
    stats_ui.text_revenue.setFont(font);
    stats_ui.text_total_game.setFont(font);
    stats_ui.time_elapsed.setFont(font);

    stats_ui.text_loss.setFillColor(sf::Color::Black);
    stats_ui.text_profit.setFillColor(sf::Color::Black);
    stats_ui.text_revenue.setFillColor(sf::Color::Black);
    stats_ui.text_total_game.setFillColor(sf::Color::Black);
    stats_ui.time_elapsed.setFillColor(sf::Color::Black);

    stats_ui.text_loss.setCharacterSize(12);
    stats_ui.text_profit.setCharacterSize(12);
    stats_ui.text_revenue.setCharacterSize(12);
    stats_ui.text_total_game.setCharacterSize(12);
    stats_ui.time_elapsed.setCharacterSize(12);

    // TODO BIKIN STRINGNYA
    stats_ui.text_loss.setString("TEXTSFSFOKFDKSJ");
    stats_ui.text_profit.setString("TEXTSFSFOKFDKSJ");
    stats_ui.text_revenue.setString("TEXTSFSFOKFDKSJ");
    stats_ui.text_total_game.setString("TEXTSFSFOKFDKSJ");
    

    sf::Vector2f text_post = sf::Vector2f(300, 10);
    stats_ui.text_revenue.setPosition(text_post);

    text_post.y += 15;
    stats_ui.text_loss.setPosition(text_post);

    text_post.y += 15;
    stats_ui.text_profit.setPosition(text_post);

    text_post.y += 15;
    stats_ui.text_total_game.setPosition(text_post);

    sf::Vector2f time_post = sf::Vector2f(550, 10);
    stats_ui.time_elapsed.setPosition(time_post);
}

void Gui::drawStats() {
    float elapsedTime = clock.getElapsedTime().asSeconds();
    stats_ui.time_elapsed.setString("Time : " + formatTime(elapsedTime));
    stats_ui.text_revenue.setString("Revenue : " + std::to_string(casino_manager.get_revenue()));
    stats_ui.text_loss.setString("Loss : " + std::to_string(casino_manager.get_loss()));
    stats_ui.text_profit.setString("Profit : " + std::to_string(casino_manager.get_profit()));
    stats_ui.text_total_game.setString("Total Game : " + std::to_string(casino_manager.get_total()));


    this->draw(stats_ui.text_revenue);
    this->draw(stats_ui.text_loss);
    this->draw(stats_ui.text_profit);
    this->draw(stats_ui.text_total_game);
    this->draw(stats_ui.time_elapsed);

}