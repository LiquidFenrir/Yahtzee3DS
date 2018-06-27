#include "states/playing.h"
#include "game/network.h"
#include "game/drawing.h"

static constexpr int PlayingMaxRollAmount = 3;

static constexpr float PlayingButtonZ = 0.5f;

static constexpr float PlayingRollButtonX = 80;
static constexpr float PlayingRollButtonY = 130;
static constexpr float PlayingRollButtonTextureWidth = 160;
static constexpr float PlayingRollButtonTextOffset = 0;

static constexpr float PlayingShowCombosButtonX = 80;
static constexpr float PlayingShowCombosButtonY = 177;
static constexpr float PlayingShowCombosButtonTextureWidth = 160;
static constexpr float PlayingShowCombosButtonTextOffset = 0;

static constexpr float PlayingComboSelectButtonX = 150;
static constexpr float PlayingComboSelectButtonYStart = 42;
static constexpr float PlayingComboSelectButtonTextureWidth = 140;
static constexpr float PlayingComboSelectButtonTextOffset = 60;


static constexpr float PlayingMinSelectedDice = 0;
static constexpr float PlayingMaxSelectedDice = diceAmount-1;

static constexpr int PlayingComboViewMinScroll = 0;
static constexpr int PlayingComboViewMaxScroll = COMBO_AMOUNT-PlayingComboViewLinesPerScreen;

static constexpr int PlayingComboViewMinSelection = 0;
static constexpr int PlayingComboViewMaxSelection = COMBO_AMOUNT-1;

static constexpr int PlayingShakerAnimationSteps = 4;
static const std::string comboViewName[COMBO_AMOUNT] = {
    "Aces:",
    "Twos:",
    "Threes:",
    "Fours:",
    "Fives:",
    "Sixes:",

    "Three of a kind:",
    "Four of a kind:",
    "Full House:",
    "Small Straight:",
    "Large Straight:",
    "Yahtzee:",

    "Chance:",
};

PlayingState::PlayingState(int playersAmount)
{
    DEBUG("PlayingState::PlayingState\n");
    DEBUG("Playing with %i players, single console\n", playersAmount);

    for(int i = 0; i < playersAmount; i++)
    {
        this->players.push_back(Player("Player " + std::to_string(i+1)));
    }

    this->playerID = -1;

    this->rollButton = Button(PlayingRollButtonX, PlayingRollButtonY, PlayingButtonZ, PlayingRollButtonTextureWidth, PlayingRollButtonTextOffset, "Roll again", std::bind(&PlayingState::roll, this));
    this->showCombos = Button(PlayingShowCombosButtonX, PlayingShowCombosButtonY, PlayingButtonZ, PlayingShowCombosButtonTextureWidth, PlayingShowCombosButtonTextOffset, "Combinations", std::bind(&PlayingState::showCombinations, this));
    for(int i = 0; i < PlayingComboViewLinesPerScreen; i++)
    {
        this->comboSelectButtons[i] = Button(PlayingComboSelectButtonX, PlayingComboSelectButtonYStart+i*40, PlayingButtonZ, PlayingComboSelectButtonTextureWidth, PlayingComboSelectButtonTextOffset, "Select", std::bind(&PlayingState::selectComboFromButton, this, i));
    }

    this->currentPlayer = -1;
    this->comboViewScroll = PlayingComboViewMinScroll;
    this->comboSelection = PlayingComboViewMinSelection;
    this->gameComplete = false;
    this->someoneLeft = false;

    srand(time(NULL));
    this->startNewTurn();
}

PlayingState::PlayingState(int playerID, std::shared_ptr<Room>& room)
{
    DEBUG("PlayingState::PlayingState\n");

    std::vector<std::string> names = room->getPlayerNames();
    this->playersAmount = room->getPlayerCount();

    DEBUG("Playing with %i players, multiple consoles\n", playersAmount);
    for(int i = 0; i < this->playersAmount; i++)
    {
        std::string playerName = names[i];
        DEBUG("Player %i has name %s\n", i+1, playerName.c_str());
        this->players.push_back(Player(playerName));
    }

    this->playerID = playerID;
    this->room = room;

    this->rollButton = Button(PlayingRollButtonX, PlayingRollButtonY, PlayingButtonZ, PlayingRollButtonTextureWidth, PlayingRollButtonTextOffset, "Roll again", std::bind(&PlayingState::roll, this));
    this->showCombos = Button(PlayingShowCombosButtonX, PlayingShowCombosButtonY, PlayingButtonZ, PlayingShowCombosButtonTextureWidth, PlayingShowCombosButtonTextOffset, "Combinations", std::bind(&PlayingState::showCombinations, this));
    for(int i = 0; i < PlayingComboViewLinesPerScreen; i++)
    {
        this->comboSelectButtons[i] = Button(PlayingComboSelectButtonX, PlayingComboSelectButtonYStart+i*40, PlayingButtonZ, PlayingComboSelectButtonTextureWidth, PlayingComboSelectButtonTextOffset, "Select", std::bind(&PlayingState::selectComboFromButton, this, i));
    }

    this->currentPlayer = -1;
    this->comboViewScroll = PlayingComboViewMinScroll;
    this->comboSelection = PlayingComboViewMinSelection;
    this->gameComplete = false;
    this->someoneLeft = false;

    srand(time(NULL));
    this->startNewTurn();
}

PlayingState::~PlayingState()
{
    DEBUG("PlayingState::~PlayingState\n");
}

void PlayingState::update()
{
    if(this->gameComplete || (this->playerID != -1 && this->someoneLeft))
    {
        if(kDown & KEY_B)
        {
            this->nextState = new MainMenuState;
        }
    }
    else
    {
        if(this->playerID != -1)
        {
            if(this->room->getPlayerCount() != this->playersAmount)
            {
                this->someoneLeft = true;
                return;
            }

            if(this->currentPlayer == static_cast<size_t>(this->playerID))
                this->room->sendLocalKeys();
            else
                this->room->getRemotePlayerKeys();
        }

        if(this->selectionMode == SELECTION_MODE_COMBO)
        {
            if(kDown & KEY_DOWN)
            {   
                this->comboSelection++;
                if(this->comboSelection > PlayingComboViewMaxSelection)
                    this->comboSelection = PlayingComboViewMaxSelection;

                if(this->comboViewScroll+this->comboSelection >= PlayingComboViewLinesPerScreen && this->comboSelection == this->comboViewScroll+PlayingComboViewLinesPerScreen)
                {
                    this->comboViewScroll++;
                    if(this->comboViewScroll > PlayingComboViewMaxScroll)
                        this->comboViewScroll = PlayingComboViewMaxScroll;
                }
            }
            else if(kDown & KEY_UP)
            {
                this->comboSelection--;
                if(this->comboSelection < PlayingComboViewMinSelection)
                    this->comboSelection = PlayingComboViewMinSelection;

                if(this->comboViewScroll > PlayingComboViewMinScroll && this->comboSelection == this->comboViewScroll-1)
                {
                    this->comboViewScroll--;
                    if(this->comboViewScroll < PlayingComboViewMinScroll)
                        this->comboViewScroll = PlayingComboViewMinScroll;
                }
            }
            else if(kDown & KEY_B)
            {
                this->selectionMode = this->modeBeforeComboViewing;
            }
            else if(kDown & KEY_A)
            {
                ComboType type = static_cast<ComboType>(this->comboSelection);
                this->selectCombo(type);
            }
            if(kDown & KEY_TOUCH)
            {
                for(int i = this->comboViewScroll; i < this->comboViewScroll+PlayingComboViewLinesPerScreen; i++)
                {
                    auto& button = this->comboSelectButtons[i];
                    if(button.isPressed())
                    {
                        if(i == this->comboSelection)
                        {
                            button.action();
                        }
                        else
                        {
                            this->comboSelection = i;
                        }
                    }
                }
            }
        }
        else
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

                    if(this->showCombos.isPressed())
                    {
                        if(this->selectionMode == SELECTION_MODE_SHOW)
                        {
                            this->showCombos.action();
                        }
                        else
                        {
                            this->selectionMode = SELECTION_MODE_SHOW;
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
                    else if(kDown & KEY_DOWN)
                    {
                        this->selectionMode = this->rollAmount < PlayingMaxRollAmount ? SELECTION_MODE_ROLL : SELECTION_MODE_SHOW;
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
                    else if(kDown & KEY_DOWN)
                    {
                        this->selectionMode = SELECTION_MODE_SHOW;
                    }
                }
                else if(this->selectionMode == SELECTION_MODE_SHOW)
                {
                    if(kDown & KEY_A)
                    {
                        this->showCombos.action();
                    }
                    else if(kDown & KEY_UP)
                    {
                        this->selectionMode = this->rollAmount < PlayingMaxRollAmount ? SELECTION_MODE_ROLL : SELECTION_MODE_DICE;
                    }
                }
            }
        }
    }
}

void PlayingState::draw()
{
    static float textScale = 0.6f;
    if(this->gameComplete)
    {
        C2D_Text congratulations, winnerText, pointsText, pressB;
        char withPoints[128] = {0};
        snprintf(withPoints, 127, "With %i points", this->winnerPoints);

        C2D_TextParse(&congratulations, dynamicBuf, "Congratulations!");
        C2D_TextOptimize(&congratulations);

        C2D_TextParse(&pointsText, dynamicBuf, withPoints);
        C2D_TextOptimize(&pointsText);

        C2D_TextParse(&winnerText, dynamicBuf, this->winner.c_str());
        C2D_TextOptimize(&winnerText);

        C2D_TextParse(&pressB, dynamicBuf, "Press \uE001 to go back.");
        C2D_TextOptimize(&pressB);

        float width = 0, height = 0;
        C2D_TextGetDimensions(&congratulations, 1.0f, 1.0f, &width, &height);
        float offset = (320/2 - width/2);

        float y = 24;
        C2D_DrawText(&congratulations, C2D_WithColor, offset, y, 0.5f, 1.0f, 1.0f, textColor);
        y += height*2 + 5;

        C2D_TextGetDimensions(&winnerText, textScale, textScale, &width, &height);
        offset = (320/2 - width/2);
        C2D_DrawText(&winnerText, C2D_WithColor, offset, y, 0.5f, textScale, textScale, textColor);
        y += height + 5;

        C2D_TextGetDimensions(&pointsText, textScale, textScale, &width, &height);
        offset = (320/2 - width/2);
        C2D_DrawText(&pointsText, C2D_WithColor, offset, y, 0.5f, textScale, textScale, textColor);
        y += height*2 + 5;
 
        C2D_TextGetDimensions(&pressB, textScale, textScale, &width, nullptr);
        offset = (320/2 - width/2);
        C2D_DrawText(&pressB, C2D_WithColor, offset, y, 0.5f, textScale, textScale, textColor);
    }
    else if(playerID != -1 && this->someoneLeft)
    {
        C2D_Text someoneLeft, pressB;
        C2D_TextParse(&someoneLeft, dynamicBuf, "Someone left the game!");
        C2D_TextOptimize(&someoneLeft);
        C2D_TextParse(&pressB, dynamicBuf, "Press \uE001 to go back.");
        C2D_TextOptimize(&pressB);

        float y = 80;
        float width = 0;
        float offset = 0;

        C2D_TextGetDimensions(&someoneLeft, 0.8f, 0.8f, &width, nullptr);
        offset = (320/2 - width/2);
        C2D_DrawText(&someoneLeft, C2D_WithColor, offset, y, 0.5f, 0.8f, 0.8f, textColor);

        y += 40;

        C2D_TextGetDimensions(&pressB, textScale, textScale, &width, nullptr);
        offset = (320/2 - width/2);
        C2D_DrawText(&pressB, C2D_WithColor, offset, y, 0.5f, textScale, textScale, textColor);
    }
    else if(this->selectionMode == SELECTION_MODE_COMBO)
    {
        // Display combos and score
        auto& player = this->players[this->currentPlayer];

        C2D_Text playerText;
        std::string name = player.getName();
        int points = player.getTotalPoints();

        char playerWithPoints[128] = {0};
        snprintf(playerWithPoints, 127, "%s: %i points", name.c_str(), points);

        C2D_TextParse(&playerText, dynamicBuf, playerWithPoints);
        C2D_TextOptimize(&playerText);

        float width = 0;
        C2D_TextGetDimensions(&playerText, textScale, textScale, &width, nullptr);
        float offset = (320/2 - width/2);

        C2D_DrawText(&playerText, C2D_WithColor, offset, 20, 0.5f, textScale, textScale, blackColor);

        for(int i = this->comboViewScroll; i < this->comboViewScroll+PlayingComboViewLinesPerScreen; i++)
        {
            int cleanI = i-this->comboViewScroll;
            float y = 52 + cleanI*40;

            C2D_Text text;
            C2D_TextParse(&text, dynamicBuf, comboViewName[i].c_str());
            C2D_TextOptimize(&text);

            ComboType type = static_cast<ComboType>(i);
            diceHand combo = player.getDiceCombo(type);
            bool completed = player.isComboCompleted(type);
            bool failed = player.isComboFailed(type);

            bool drawDice = false;
            u32 color = blackColor;
            if(failed)
            {
                drawDice = true;
                color = failedTextColor;
            }
            else if(completed)
            {
                drawDice = true;
                color = greyedOutTextColor;
            }

            C2D_DrawText(&text, C2D_WithColor, 24, y, 0.4f, textScale, textScale, color);
            if(drawDice)
            {
                for(int j = PlayingMinSelectedDice; j <= PlayingMaxSelectedDice; j++)
                {
                    float x = 140 + j*32;
                    C2D_DrawImageAt(C2D_SpriteSheetGetImage(spritesheet, sprites_dice_1_idx+combo[j]-1), x, y-8, 0.5f);
                }
            }
            else
            {
                this->comboSelectButtons[cleanI].draw();
                if(cleanI == this->comboSelection-this->comboViewScroll)
                    this->comboSelectButtons[cleanI].drawOverlay();
            }
        }
    }
    else
    {
        this->drawDice(this->drawShakerAnimation());  // draw the unlocked dice as missing during the shaker animation 

        if(!this->shakerAnimationTime)
        {
            if(this->rollAmount < PlayingMaxRollAmount)
            {
                this->rollButton.draw();
                if(this->selectionMode == SELECTION_MODE_ROLL)
                    this->rollButton.drawOverlay();
            }

            this->showCombos.draw();
            if(this->selectionMode == SELECTION_MODE_SHOW)
                this->showCombos.drawOverlay();
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
    bool everyoneFinished = true;
    std::string winnerName;
    int highestPoints = -1;

    for(auto& player : this->players)
    {
        if(!player.hasFinished())
        {
            everyoneFinished = false;
            break;
        }
        else
        {
            int playerPoints = player.getTotalPoints();
            if(playerPoints > highestPoints)
            {
                winnerName = "The winner is: " + player.getName();
                highestPoints = playerPoints;
            }
            else if(playerPoints == highestPoints)
            {
                std::string tieBetween = "Tie between: ";
                std::string appended = " and " + player.getName();
                if(winnerName.compare(tieBetween))  // If winnerName doesnt start with tieBetween
                {
                    winnerName = tieBetween + winnerName.substr(15);
                }
                winnerName += appended;
            }
        }
    }

    if(everyoneFinished)
    {
        this->gameComplete = true;
        this->winner = winnerName;
        this->winnerPoints = highestPoints;
        return;
    }

    for(int i = 0; i < diceAmount; i++)
        this->locked[i] = false;

    do {
        this->currentPlayer++;
        this->currentPlayer %= this->players.size();
    } while(this->players[this->currentPlayer].hasFinished());

    this->rollAmount = 0;
    this->roll();
}

bool PlayingState::drawShakerAnimation()
{
    float x = 32;
    float y = 24;
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
        float y = 86;
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

void PlayingState::selectComboFromButton(int buttonID)
{
    ComboType actualCombo = static_cast<ComboType>(buttonID+this->comboViewScroll);
    this->selectCombo(actualCombo);
}

void PlayingState::selectCombo(ComboType type)
{
    auto& player = this->players[this->currentPlayer];
    if(!player.isComboCompleted(type))
    {
        player.setCombo(type, this->hand);
        this->startNewTurn();
    }
}

void PlayingState::showCombinations()
{
    this->comboViewScroll = PlayingComboViewMinScroll;
    this->comboSelection = PlayingComboViewMinSelection;
    this->modeBeforeComboViewing = this->selectionMode;
    this->selectionMode = SELECTION_MODE_COMBO;
}
