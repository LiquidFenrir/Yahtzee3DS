#pragma once

#include "common.h"

typedef std::array<int, diceAmount> diceHand;

struct DicePointCombo
{
    diceHand combo;
    int points;
};

enum ComboType
{
    COMBO_ACES,
    COMBO_TWOS,
    COMBO_THREES,
    COMBO_FOURS,
    COMBO_FIVES,
    COMBO_SIXES,

    COMBO_THREE_OF_A_KIND,
    COMBO_FOUR_OF_A_KIND,
    COMBO_FULL_HOUSE,
    COMBO_SMALL_STRAIGHT,
    COMBO_LARGE_STRAIGHT,
    COMBO_YAHTZEE,

    COMBO_CHANCE,

    COMBO_AMOUNT
};

class Player
{
    public:
        Player();
        ~Player();

        int getTotalPoints();

        void setSingleType(diceHand diceValues, ComboType type);

        void setAces(diceHand diceValues);
        void setTwos(diceHand diceValues);
        void setThrees(diceHand diceValues);
        void setFours(diceHand diceValues);
        void setFives(diceHand diceValues);
        void setSixes(diceHand diceValues);

        void setThreeOfAKind(diceHand diceValues);
        void setFourOfAKind(diceHand diceValues);
        void setFullHouse(diceHand diceValues);
        void setSmallStraight(diceHand diceValues);
        void setLargeStraight(diceHand diceValues);
        void setYathzee(diceHand diceValues);
        void setChance(diceHand diceValues);

        diceHand getDiceCombo(ComboType combo);
        bool isComboCompleted(ComboType combo);
        bool isComboFailed(ComboType combo);

    private:
        std::array<DicePointCombo, COMBO_AMOUNT> combos;
        std::array<bool, COMBO_AMOUNT> completedCombos;
        std::array<bool, COMBO_AMOUNT> failedCombos;
};
