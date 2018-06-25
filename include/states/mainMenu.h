#pragma once

#include "states/states.h"
#include "game/button.h"

enum MainMenuButtons
{
    MAINMENU_BUTTONS_SINGLEPLAYER,
    MAINMENU_BUTTONS_MULTIPLAYER,
    MAINMENU_BUTTONS_RULES,
    MAINMENU_BUTTONS_QUIT,
    
    MAINMENU_BUTTONS_AMOUNT
};

class MainMenuState : public State
{
    public:
        MainMenuState();
        ~MainMenuState();

        void update();
        void draw();

    private:
        std::array<Button, MAINMENU_BUTTONS_AMOUNT> buttons;
        int selectedButton;
};
