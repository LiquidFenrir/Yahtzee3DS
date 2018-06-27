#pragma once

#include "states/states.h"
#include "game/button.h"
#include "game/network.h"

constexpr int NetworkListRoomsPerScreen = 3;

enum NetworkListButtons
{
    NETWORK_LIST_REFRESH,
    NETWORK_LIST_BACK,

    NETWORK_LIST_BUTTONS_AMOUNT
};

enum NetworkListSelectModes
{
    NETWORK_LIST_SELECT_ROOM,
    NETWORK_LIST_SELECT_BUTTON, 
};

class NetworkListState : public State
{
    public:
        NetworkListState();
        ~NetworkListState();

        void update();
        void draw();

    private:
        void joinRoom(int i);
        void refreshList();
        void goBack();
        void joinSelectedRoom();

        std::array<Button, NETWORK_LIST_BUTTONS_AMOUNT> buttons;
        std::array<Button, NetworkListRoomsPerScreen> joinButtons;
        std::vector<std::shared_ptr<Room>> rooms;
        int selectedButton, selectedRoom;
        int roomsScroll;
        NetworkListSelectModes mode;
};
