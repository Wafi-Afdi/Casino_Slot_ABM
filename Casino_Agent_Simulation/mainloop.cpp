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

            switch (event.type)
            {
                // window closed
            case sf::Event::Closed:
                this->close();
                break;

                // key pressed
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Num1) {
                    sim_utils::game_speed = 1;
                }
                if (event.key.code == sf::Keyboard::Num2) {
                    sim_utils::game_speed = 5;
                }
                if (event.key.code == sf::Keyboard::Num3) {
                    sim_utils::game_speed = 10;
                }
                break;

                // we don't process other types of events
            default:
                break;
            }

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