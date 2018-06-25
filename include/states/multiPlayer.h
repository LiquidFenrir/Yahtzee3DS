#pragma once

#include "states/states.h"
#include "game/button.h"

enum MultiplayerButtons
{
    MULTIPLAYER_BUTTONS_LOCAL,
    MULTIPLAYER_BUTTONS_NETWORK,
    MULTIPLAYER_BUTTONS_BACK,

    MULTIPLAYER_BUTTONS_AMOUNT
};

class MultiplayerState : public State
{
    public:
        MultiplayerState();
        ~MultiplayerState();

        void update();
        void draw();

    private:
        std::array<Button, MULTIPLAYER_BUTTONS_AMOUNT> buttons;
        int selectedButton;
};
