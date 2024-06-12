#include "Game_Manager/gui.h"
#include "Game_Manager/utils.h"

void Gui::mainloop() {
    sf::Clock deltaClock;
    while (this->isOpen())
    {        
        sf::Time dt = deltaClock.restart();
        this->time_passed += dt.asSeconds() * sim_utils::get_game_speed(); 
        sf::Event event;
        while (this->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                this->close();
        }
        
        this->update();
        
        this->clear(sf::Color::White);

        this->drawSlot();
        this->drawPlayer();
        this->drawText();
        this->drawLegend();
        this->drawStats();

        this->display();
    }
}