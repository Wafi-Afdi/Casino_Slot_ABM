#include "Game_Manager/gui.h"

void Gui::mainloop() {
    while (this->isOpen())
    {        
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

        this->display();
    }
}