#include "states/playing.h"
#include "game/drawing.h"

static constexpr int PlayingMaxRollAmount = 3;
static constexpr float PlayingRollButtonX = 40;
static constexpr float PlayingRollButtonY = 160;

static constexpr float PlayingMinSelectedDice = 0;
static constexpr float PlayingMaxSelectedDice = diceAmount-1;

static constexpr int PlayingShakerAnimationSteps = 4;

PlayingState::PlayingState(int playersAmount, getKeysType getKeys, unsigned int seed)
{
    DEBUG("PlayingState::PlayingState\n");
    DEBUG("Playing with %i players\n", playersAmount);

    for(int i = 0; i < playersAmount; i++)
    {
        this->players.push_back(Player());
    }

    this->getKeys = getKeys;

    this->rollButton = Button(PlayingRollButtonX, PlayingRollButtonY, 0.5f, "Roll again", std::bind(&PlayingState::roll, this));

    srand(seed);
    this->startNewTurn();
}

PlayingState::~PlayingState()
{
    DEBUG("PlayingState::~PlayingState\n");
}

void PlayingState::update()
{
    if(!this->shakerAnimationTime)
    {
        if(kDown & KEY_TOUCH)
        {
            if(this->rollButton.isPressed() && this->rollAmount < PlayingMaxRollAmount)
            {
                if(this->selectionMode == SELECTION_MODE_ROLL)
                {
                    this->rollButton.action();
                }
                else
                {
                    this->selectionMode = SELECTION_MODE_ROLL;
                }
            }

            for(int i = PlayingMinSelectedDice; i <= PlayingMaxSelectedDice; i++)
            {
                int x = 64 + i*40;
                int y = 100;
                int size = 32;
                if(touch.px >= x && touch.px <= x + size && touch.py >= y && touch.py <= y + size)
                {
                    if(this->selectedDice == i && this->selectionMode == SELECTION_MODE_DICE)
                    {
                        goto toggleLock;
                    }
                    else
                    {
                        this->selectedDice = i;
                    }
                    this->selectionMode = SELECTION_MODE_DICE;
                    break;
                }
            }
        }
        else if(this->selectionMode == SELECTION_MODE_DICE)
        {
            if(kDown & KEY_A)
            {
                toggleLock:
                this->locked[this->selectedDice] = !this->locked[this->selectedDice];
            }
            else if(kDown & KEY_LEFT)
            {
                this->selectedDice--;
                if(this->selectedDice < PlayingMinSelectedDice)
                    this->selectedDice = PlayingMaxSelectedDice;
            }
            else if(kDown & KEY_RIGHT)
            {
                this->selectedDice++;
                if(this->selectedDice > PlayingMaxSelectedDice)
                    this->selectedDice = PlayingMinSelectedDice;
            }
            else if(kDown & KEY_DOWN && this->rollAmount < PlayingMaxRollAmount)
            {
                this->selectionMode = SELECTION_MODE_ROLL;
            }
        }
        else if(this->selectionMode == SELECTION_MODE_ROLL)
        {
            if(kDown & KEY_A)
            {
                this->rollButton.action();
            }
            else if(kDown & KEY_UP)
            {
                this->selectionMode = SELECTION_MODE_DICE;
            }
        }
    }
}

void PlayingState::draw()
{
    // if(this->viewingCombos)
    // {
        // this->players[this->selectedPlayer].displayCombosAndScore();
    // }
    // else
    {
        this->drawDice(this->drawShakerAnimation());  // draw the unlocked dice as missing during the shaker animation 

        if(this->rollAmount < PlayingMaxRollAmount && !this->shakerAnimationTime)
        {
            this->rollButton.draw();
            if(this->selectionMode == SELECTION_MODE_ROLL)
            {
                C2D_DrawImageAt(C2D_SpriteSheetGetImage(spritesheet, sprites_button_overlay_idx), PlayingRollButtonX, PlayingRollButtonY, 0.6f);
            }
        }
    }
}

void PlayingState::roll()
{
    if(this->rollAmount >= PlayingMaxRollAmount)
        return;

    for(int i = PlayingMinSelectedDice; i <= PlayingMaxSelectedDice; i++)
    {
        if(this->locked[i])
            continue;

        this->hand[i] = (rand() % diceMaxValue) + diceMinValue;
    }

    this->rollAmount++;
    this->selectionMode = SELECTION_MODE_DICE;
    this->selectedDice = PlayingMinSelectedDice;
    this->shakerAnimationTime = osGetTime();
}

void PlayingState::startNewTurn()
{
    for(int i = 0; i < diceAmount; i++)
        this->locked[i] = false;

    this->rollAmount = 0;
    this->roll();
}

bool PlayingState::drawShakerAnimation()
{
    float x = 32;
    float y = 32;
    if(this->shakerAnimationTime)
    {
        u64 time = osGetTime() - this->shakerAnimationTime;
        int step = time/100;

        if(step > 3*PlayingShakerAnimationSteps)
        {
            this->shakerAnimationTime = 0;
        }
        else
        {
            switch(step % PlayingShakerAnimationSteps)
            {
                case 0:
                    break;
                case 1:
                    x += 4;
                    break;
                case 2:
                    x += 8;
                    break;
                case 3:
                    x += 4;
                    break;
            }
        }
    }

    C2D_DrawImageAt(C2D_SpriteSheetGetImage(spritesheet, sprites_shaker_idx), x, y, 0.5f);
    return this->shakerAnimationTime != 0;
}

void PlayingState::drawDice(bool lockedOnly)
{
    for(int i = PlayingMinSelectedDice; i <= PlayingMaxSelectedDice; i++)
    {
        float x = 64 + i*40;
        float y = 100;
        if((lockedOnly && this->locked[i]) || !lockedOnly)
            C2D_DrawImageAt(C2D_SpriteSheetGetImage(spritesheet, sprites_dice_1_idx+hand[i]-1), x, y, 0.5f);

        if(this->locked[i])
            C2D_DrawImageAt(C2D_SpriteSheetGetImage(spritesheet, sprites_dice_locked_idx), x, y, 0.6f);

        if(!this->shakerAnimationTime)
        {
            if(this->selectionMode == SELECTION_MODE_DICE && i == this->selectedDice)
                C2D_DrawImageAt(C2D_SpriteSheetGetImage(spritesheet, sprites_dice_selected_idx), x, y, 0.7f);
        }
    }
}
