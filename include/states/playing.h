#pragma once

#include "states/states.h"
#include "game/player.h"
#include "game/button.h"

typedef void(*getKeysType)();

enum SelectionMode
{
    SELECTION_MODE_DICE,
    SELECTION_MODE_ROLL,
};

class PlayingState : public State
{
    public:
        PlayingState(int playersAmount, getKeysType getKeys, unsigned int seed);
        ~PlayingState();

        void update();
        void draw();

    private:
        void roll();
        void startNewTurn();

        bool drawShakerAnimation();
        void drawDice(bool lockedOnly);

        SelectionMode selectionMode;
        diceHand hand;
        std::array<bool, diceAmount> locked;
        int rollAmount;

        u64 shakerAnimationTime;
        Button rollButton;

        int selectedPlayer;
        int selectedDice;

        size_t currentPlayer;
        std::vector<Player> players;
        getKeysType getKeys;
};
