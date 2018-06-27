#pragma once

#include "states/states.h"
#include "game/button.h"
#include "game/network.h"

enum WaitingForPlayersButtons
{
    WAITING_FOR_PLAYERS_READY,
    WAITING_FOR_PLAYERS_NOT_READY,
    WAITING_FOR_PLAYERS_BACK,

    WAITING_FOR_PLAYERS_BUTTONS_AMOUNT
};

class WaitingForPlayersState : public State
{
    public:
        WaitingForPlayersState();
        WaitingForPlayersState(int playerID, std::shared_ptr<Room>& room);
        ~WaitingForPlayersState();

        void update();
        void draw();

    private:
        void setReady();
        void setNotReady();
        void goBack();

        std::array<Button, WAITING_FOR_PLAYERS_BUTTONS_AMOUNT> buttons;
        std::shared_ptr<Room> room;
        int playerID;
        int selectedButton;
        bool ownerLeft;
        bool ready;
};
