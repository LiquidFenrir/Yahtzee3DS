#include "game/player.h"

Player::Player()
{

}

Player::~Player()
{

}

int Player::getTotalPoints()
{
    int points = 0;
    for(auto& combo : this->combos)
    {
        points += combo.points;
    }
    return points;
}


void Player::setSingleType(diceHand diceValues, ComboType type)
{
    DicePointCombo* combo = &this->combos[type];
    combo->combo = diceValues;
    this->completedCombos[type] = true;

    int target = static_cast<int>(type)+1; // Enum starts at 0, so +1 is needed to bring back to valid dice range [1; 6]
    int count = std::count(diceValues.begin(), diceValues.end(), target);

    bool failed = count == 0;
    if(failed)
    {
        this->failedCombos[type] = true;
    }
    else
    {
        combo->points = count*target;
    }
}


void Player::setAces(diceHand diceValues)
{
    this->setSingleType(diceValues, COMBO_ACES);
}

void Player::setTwos(diceHand diceValues)
{
    this->setSingleType(diceValues, COMBO_TWOS);
}

void Player::setThrees(diceHand diceValues)
{
    this->setSingleType(diceValues, COMBO_THREES);
}

void Player::setFours(diceHand diceValues)
{
    this->setSingleType(diceValues, COMBO_FOURS);
}

void Player::setFives(diceHand diceValues)
{
    this->setSingleType(diceValues, COMBO_FIVES);
}

void Player::setSixes(diceHand diceValues)
{
    this->setSingleType(diceValues, COMBO_SIXES);
}


void Player::setThreeOfAKind(diceHand diceValues)
{
    this->combos[COMBO_THREE_OF_A_KIND].combo = diceValues;
    this->completedCombos[COMBO_THREE_OF_A_KIND] = true;

    for(int target = diceMinValue; target <= diceMaxValue; target++)
    {
        int count = std::count(diceValues.begin(), diceValues.end(), target);
        if(count >= 3)
        {
            this->combos[COMBO_CHANCE].points = std::accumulate(diceValues.begin(), diceValues.end(), 0);
            return;
        }
    }

    // Will only be set if it couldnt find a combo of 3 or more
    this->failedCombos[COMBO_FOUR_OF_A_KIND] = true;
}

void Player::setFourOfAKind(diceHand diceValues)
{
    this->combos[COMBO_FOUR_OF_A_KIND].combo = diceValues;
    this->completedCombos[COMBO_FOUR_OF_A_KIND] = true;

    for(int target = diceMinValue; target <= diceMaxValue; target++)
    {
        int count = std::count(diceValues.begin(), diceValues.end(), target);
        if(count >= 4)
        {
            this->combos[COMBO_CHANCE].points = std::accumulate(diceValues.begin(), diceValues.end(), 0);
            return;
        }
    }

    // Will only be set if it couldnt find a combo of 4 or more
    this->failedCombos[COMBO_FOUR_OF_A_KIND] = true;
}

void Player::setFullHouse(diceHand diceValues)
{
    this->combos[COMBO_FULL_HOUSE].combo = diceValues;
    this->completedCombos[COMBO_FULL_HOUSE] = true;

    int pair = 0, triple = 0;
    for(int target = diceMinValue; target <= diceMaxValue; target++)
    {
        int count = std::count(diceValues.begin(), diceValues.end(), target);
        if(count == 2)
        {
            pair = target;
        }
        else if(count == 3)
        {
            triple = target;
        }
    }

    if(pair && triple)
    {
        this->combos[COMBO_FULL_HOUSE].points = 25;
    }
    else
    {
        this->failedCombos[COMBO_FULL_HOUSE] = true;
    }
}

void Player::setSmallStraight(diceHand diceValues)
{
    this->combos[COMBO_SMALL_STRAIGHT].combo = diceValues;
    this->completedCombos[COMBO_SMALL_STRAIGHT] = true;

    for(int target = diceMinValue; target <= diceMaxValue-1; target++)
    {
        int count = std::count(diceValues.begin(), diceValues.end(), target);
        if(count != 1)
        {
            this->failedCombos[COMBO_SMALL_STRAIGHT] = true;
            return;
        }
    }

    this->combos[COMBO_SMALL_STRAIGHT].points = 30;
}

void Player::setLargeStraight(diceHand diceValues)
{
    this->combos[COMBO_LARGE_STRAIGHT].combo = diceValues;
    this->completedCombos[COMBO_LARGE_STRAIGHT] = true;

    for(int target = diceMinValue+1; target <= diceMaxValue; target++)
    {
        int count = std::count(diceValues.begin(), diceValues.end(), target);
        if(count != 1)
        {
            this->failedCombos[COMBO_LARGE_STRAIGHT] = true;
            return;
        }
    }

    this->combos[COMBO_LARGE_STRAIGHT].points = 40;
}

void Player::setYathzee(diceHand diceValues)
{
    this->combos[COMBO_YAHTZEE].combo = diceValues;
    this->completedCombos[COMBO_YAHTZEE] = true;

    for(int target = diceMinValue; target <= diceMaxValue; target++)
    {
        int count = std::count(diceValues.begin(), diceValues.end(), target);
        if(count == 5)
        {
            this->combos[COMBO_YAHTZEE].points = 50;
            return;
        }
    }

    // Will only be set if it couldnt find a combo of 5
    this->failedCombos[COMBO_YAHTZEE] = true;
}

void Player::setChance(diceHand diceValues)
{
    this->combos[COMBO_CHANCE].combo = diceValues;
    this->combos[COMBO_CHANCE].points = std::accumulate(diceValues.begin(), diceValues.end(), 0);
    this->completedCombos[COMBO_CHANCE] = true;
}


diceHand Player::getDiceCombo(ComboType combo)
{
    return this->combos[combo].combo;
}

bool Player::isComboCompleted(ComboType combo)
{
    return this->completedCombos[combo];
}

bool Player::isComboFailed(ComboType combo)
{
    return this->failedCombos[combo];
}
