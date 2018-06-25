#pragma once

#include "states/states.h"
#include "game/player.h"

typedef void(*getKeysType)();

class PlayingState : public State
{
    public:
        PlayingState(int playersAmount, getKeysType getKeys);
        ~PlayingState();

        void update();
        void draw();

    private:
        diceHand hand;
        std::array<bool, diceAmount> locked;

        std::vector<Player> players;
        getKeysType getKeys;
};
