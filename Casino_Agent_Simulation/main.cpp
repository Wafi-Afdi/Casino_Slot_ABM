#include <cstdlib>
#include "Game_Manager/gui.h"

int main() {
    std::srand(std::time(0));

    Gui gui("SFML window");
    gui.mainloop();
}