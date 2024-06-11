#include "Game_Manager/gui.h"
#include "Casino/player.h"

#define _HORIZONTAL_WINDOW 800
#define _VERTICAL_WINDOW 600

#define _CIRCLE_RADIUS 20.0f

void Gui::update() {
    for (int i = 0; i < 4; i++) {
        if (!slot[i]) {

            sf::CircleShape circle;
            circle.setRadius(_CIRCLE_RADIUS);

            po_vector.push_back(
                { 
                    new Player(i, 0.0f, 500.0f, Gui::getTargetX(i), Gui::getTargetY(), Gui::getXExit(), Gui::getYExit()),
                    circle
                }
            );
            this->casino_manager.add_player();

            slot[i] = true;
        }
    }

    for (auto it = po_vector.begin(); it != po_vector.end();) {
        
        if (it->player->getState() == OUT) {
            delete it->player;
            it = po_vector.erase(it);
        }

        else {

            switch (it->player->getState()) {
                case TOSLOT:
                    it->circle.setPosition(sf::Vector2f(it->player->getXToSlot(), it->player->getYToSlot()));
                    break;

                case ONSLOT:
                    it->player->play(casino_manager);
                    break;

                case PLAYING:
                    it->player->check();
                    break;

                case RESULT:
                    this->result(it);
                    break;

                case EXIT:
                    it->circle.setPosition(sf::Vector2f(it->player->getXToExit(), it->player->getYToExit()));
                    break;
                }


            ++it;
        }
    }
}