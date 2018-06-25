#include "states/playing.h"
#include "game/drawing.h"

PlayingState::PlayingState(int playersAmount, getKeysType getKeys)
{
    DEBUG("PlayingState::PlayingState\n");
    DEBUG("Playing with %i players\n", playersAmount);

    for(int i = 0; i < playersAmount; i++)
    {
        this->players.push_back(Player());
    }
    
    this->getKeys = getKeys;
}

PlayingState::~PlayingState()
{
    DEBUG("PlayingState::~PlayingState\n");
}

void PlayingState::update()
{

}

void PlayingState::draw()
{
    if(this->viewingCombos)
    {
        this->players[this->selectedPlayer].displayCombosAndScore();
    }
    else
    {
        
    }
}
