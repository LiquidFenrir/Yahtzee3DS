#include "states/localMultiPlayer.h"
#include "game/drawing.h"

static constexpr int LocalMultiplayerMinPlayers = diceMinValue;
static constexpr int LocalMultiplayerMaxPlayers = diceMaxValue;

static constexpr float diceScale = 3.0f;

static constexpr float diceX = (320-32*diceScale)/2;
static constexpr float diceY = (240-32*diceScale)/2;

static constexpr float topArrowY = diceY - 32;
static constexpr float bottomArrowY = diceY + diceScale*32;

LocalMultiplayerState::LocalMultiplayerState()
{
    DEBUG("LocalMultiplayerState::LocalMultiplayerState\n");
    this->playersAmount = LocalMultiplayerMinPlayers;
    this->diceTouched = 0;
}

LocalMultiplayerState::~LocalMultiplayerState()
{
    DEBUG("LocalMultiplayerState::~LocalMultiplayerState\n");
}

void LocalMultiplayerState::update()
{
    if(kDown & KEY_A)
    {
        startPlaying:
        DEBUG("Start playing local multiplayer, players: %i\n", this->playersAmount);
        this->nextState = new PlayingState(this->playersAmount, 0);
    }
    else if(kDown & KEY_DOWN)
    {
        playerAmountDecrement:
        this->playersAmount--;
        if(this->playersAmount < LocalMultiplayerMinPlayers)
            this->playersAmount = LocalMultiplayerMinPlayers;
    }
    else if(kDown & KEY_UP)
    {
        playerAmountIncrement:
        this->playersAmount++;
        if(this->playersAmount > LocalMultiplayerMaxPlayers)
            this->playersAmount = LocalMultiplayerMaxPlayers;
    }
    else if(kDown & KEY_LEFT)
    {
        this->playersAmount = LocalMultiplayerMinPlayers;
    }
    else if(kDown & KEY_RIGHT)
    {
        this->playersAmount = LocalMultiplayerMaxPlayers;
    }
    else if(kDown & KEY_B)
    {
        this->nextState = new MultiplayerState;
    }
    else if(kDown & KEY_TOUCH)
    {
        if(touch.px >= diceX && touch.px <= diceX + (diceScale*32) && touch.py >= diceY && touch.py <= diceY + (diceScale*32))
        {
            if(this->diceTouched)
            {
                goto startPlaying;
            }
            else
            {
                this->diceTouched = osGetTime();
            }
        }
        else if(touch.px >= 148 && touch.px <= 172)
        {
            // Top arrow
            if(touch.py >= topArrowY+8 && touch.py <= topArrowY+32-4)
            {
                goto playerAmountIncrement;
            }
            // Bottom arrow
            else if(touch.py >= bottomArrowY+4 && touch.py <= bottomArrowY+32-8)
            {
                goto playerAmountDecrement;
            }
        }
    }
    
    // Need to double tap the dice in less than 0.2 seconds
    if(this->diceTouched)
    {
        if(osGetTime() - this->diceTouched >= 200)
        {
            this->diceTouched = 0;
        }
    }
}

void LocalMultiplayerState::draw()
{
    float diceScale = 3.0f;
    C2D_DrawImageAt(C2D_SpriteSheetGetImage(spritesheet, sprites_dice_1_idx+this->playersAmount-LocalMultiplayerMinPlayers), diceX, diceY, 0.5f, nullptr, diceScale, diceScale);

    C2D_DrawImageAt(C2D_SpriteSheetGetImage(spritesheet, sprites_arrow_idx), (320-32)/2, topArrowY, 0.5f, nullptr, 1.0f, 1.0f);
    C2D_DrawImageAt(C2D_SpriteSheetGetImage(spritesheet, sprites_arrow_idx), (320-32)/2, bottomArrowY, 0.5f, nullptr, 1.0f, -1.0f);
}
