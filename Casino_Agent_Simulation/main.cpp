#include <cstdlib>
#include "Game_Manager/gui.h"
#include "Game_Manager/utils.h"

int main() {
    std::srand(std::time(0));

    Gui gui("SFML window");
    gui.mainloop();
}