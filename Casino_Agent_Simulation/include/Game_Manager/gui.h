#pragma once

#include <SFML/Graphics.hpp>
#include "Casino/player.h"
#include "Casino/house_casino.h"

struct PlayerObject{
	Player* player;
	sf::CircleShape circle;
};

class Gui : public sf::RenderWindow {
	private:
		bool slot[4];

		sf::RectangleShape square[4];
		sf::Sprite slotSprite [4];

		std::vector<PlayerObject> po_vector;
		std::vector<sf::Text*> text_vector;

		void setup();
		void update();
		void winLabel(int index);
		void loseLabel(int index);
		void resultLabel(bool win, int index);
		void drawSlot();
		void drawText();
		void drawPlayer();
		void drawStats();
		void result(std::vector<PlayerObject>::iterator&);

		sf::Font font;
		sf::Texture slotTexture;

		Casino_Manager casino_manager;

	public:
		Gui(const char*);

		void mainloop();

		static float getTargetX(int index);
		static float getTargetY();
		static float getXExit();
		static float getYExit();
};
