#pragma once

#include <SFML/Graphics.hpp>
#include "Casino/player.h"
#include "Casino/house_casino.h"


struct PlayerObject{
	Player* player;
	sf::CircleShape circle;
};

struct Legend {
	sf::CircleShape circle_stop;
	sf::CircleShape circle_broke;
	sf::Text text_broke;
	sf::Text text_stop;
};

struct Stats {
	sf::Text text_revenue;
	sf::Text text_loss;
	sf::Text text_total_game;
	sf::Text text_profit;
	sf::Text time_elapsed;
	sf::Text text_house_edge;
};

class Gui : public sf::RenderWindow {
	private:
		bool slot[4];

		sf::RectangleShape square[4];
		sf::Sprite slotSprite[4];

		std::vector<PlayerObject> po_vector;
		std::vector<sf::Text*> text_vector;

		sf::Clock clock;
		float time_passed = 0.0;
		

		void setup();
		void update();
		void winLabel(int index, int result);
		void loseLabel(int index);
		void resultLabel(int win, int index);
		void drawSlot();
		void drawText();
		void drawPlayer();
		void drawLegend();
		void drawStats();
		void result(std::vector<PlayerObject>::iterator&);
		void setup_stats();

		sf::Font font;
		sf::Texture slotTexture;

		Casino_Manager casino_manager;

		Legend legend_ui;
		Stats stats_ui;

		

	public:
		Gui(const char*);

		void mainloop();

		static float getTargetX(int index);
		static float getTargetY();
		static float getXExit();
		static float getYExit();

};