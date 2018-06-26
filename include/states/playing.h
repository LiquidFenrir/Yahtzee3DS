#pragma once

#include "states/states.h"
#include "game/player.h"
#include "game/button.h"

typedef void(*getKeysType)();
constexpr int PlayingComboViewLinesPerScreen = 4;

enum SelectionMode
{
    SELECTION_MODE_DICE,
    SELECTION_MODE_ROLL,
    SELECTION_MODE_SHOW,
    SELECTION_MODE_COMBO,
};

class PlayingState : public State
{
    public:
        PlayingState(int playersAmount, unsigned int seed);
        PlayingState(int playersAmount, std::vector<std::string> names, getKeysType getKeys, unsigned int seed);
        ~PlayingState();

        void update();
        void draw();

    private:
        void roll();
        void startNewTurn();

        bool drawShakerAnimation();
        void drawDice(bool lockedOnly);

        void showCombinations();

        SelectionMode selectionMode;
        SelectionMode modeBeforeComboViewing;
        int selectedDice;
        diceHand hand;
        std::array<bool, diceAmount> locked;
        int rollAmount;

        u64 shakerAnimationTime;
        Button rollButton, showCombos;
        std::array<Button, PlayingComboViewLinesPerScreen> comboSelectButtons;
        void selectComboFromButton(int buttonID);
        void selectCombo(ComboType type);

        int comboViewScroll;
        int comboSelection;

        bool gameComplete;
        std::string winner;
        int winnerPoints;

        size_t currentPlayer;
        std::vector<Player> players;
        getKeysType getKeys;
};
