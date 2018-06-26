#pragma once

#include "states/states.h"
#include "game/button.h"
#include "game/network.h"

enum NetworkMultiplayerButtons
{
    NETWORK_MULTIPLAYER_BUTTONS_CREATE_ROOM,
    NETWORK_MULTIPLAYER_BUTTONS_LIST_ROOMS,
    NETWORK_MULTIPLAYER_BUTTONS_BACK,

    NETWORK_MULTIPLAYER_BUTTONS_AMOUNT
};

class NetworkMultiplayerState : public State
{
    public:
        NetworkMultiplayerState();
        ~NetworkMultiplayerState();

        void update();
        void draw();

    private:
        void createRoom();
        void listRooms();
        void goBack();

        std::array<Button, NETWORK_MULTIPLAYER_BUTTONS_AMOUNT> buttons;
        int selectedButton;
};
