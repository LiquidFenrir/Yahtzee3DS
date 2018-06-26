#include "states/multiPlayer.h"
#include "game/drawing.h"

static constexpr float WaitingForPlayersButtonsY = 172;
static constexpr float WaitingForPlayersButtonsTextureWidth = 120;
static constexpr float WaitingForPlayersButtonsZ = 0.5f;

WaitingForPlayersState::WaitingForPlayersState()
{
    DEBUG("WaitingForPlayersState::WaitingForPlayersState\n");
    this->buttons[WAITING_FOR_PLAYERS_READY] = Button(168, WaitingForPlayersButtonsY, WaitingForPlayersButtonsZ, WaitingForPlayersButtonsTextureWidth, 68, "Ready", std::bind(&WaitingForPlayersState::setReady, this));
    this->buttons[WAITING_FOR_PLAYERS_NOT_READY] = Button(168, WaitingForPlayersButtonsY, WaitingForPlayersButtonsZ, WaitingForPlayersButtonsTextureWidth, 68, "Waiting", std::bind(&WaitingForPlayersState::setNotReady, this));
    this->buttons[WAITING_FOR_PLAYERS_BACK] = Button(32, WaitingForPlayersButtonsY, WaitingForPlayersButtonsZ, WaitingForPlayersButtonsTextureWidth, -66, "Back", std::bind(&WaitingForPlayersState::goBack, this));
    this->selectedButton = static_cast<int>(WAITING_FOR_PLAYERS_BACK);

    std::shared_ptr<Room> room(new Room);
    this->room = room;
    this->playerID = 0;
    this->ready = false;
}

WaitingForPlayersState::WaitingForPlayersState(int playerID, std::shared_ptr<Room>& room)
{
    DEBUG("WaitingForPlayersState::WaitingForPlayersState\n");
    this->buttons[WAITING_FOR_PLAYERS_READY] = Button(168, WaitingForPlayersButtonsY, WaitingForPlayersButtonsZ, WaitingForPlayersButtonsTextureWidth, 68, "Ready", std::bind(&WaitingForPlayersState::setReady, this));
    this->buttons[WAITING_FOR_PLAYERS_NOT_READY] = Button(168, WaitingForPlayersButtonsY, WaitingForPlayersButtonsZ, WaitingForPlayersButtonsTextureWidth, 68, "Waiting", std::bind(&WaitingForPlayersState::setNotReady, this));
    this->buttons[WAITING_FOR_PLAYERS_BACK] = Button(32, WaitingForPlayersButtonsY, WaitingForPlayersButtonsZ, WaitingForPlayersButtonsTextureWidth, -66, "Back", std::bind(&WaitingForPlayersState::goBack, this));
    this->selectedButton = static_cast<int>(WAITING_FOR_PLAYERS_BACK);

    this->room = room;
    this->playerID = playerID;
    this->ready = false;
}

WaitingForPlayersState::~WaitingForPlayersState()
{
    DEBUG("WaitingForPlayersState::~WaitingForPlayersState\n");
}

void WaitingForPlayersState::update()
{
    if(this->room->everyoneReady())  // if any player isn't ready, don't start the game
    {
        this->nextState = new PlayingState(this->playerID, this->room, 0);
        return;
    }

    int backButtonID = static_cast<int>(WAITING_FOR_PLAYERS_BACK);
    auto& backButton = this->buttons[backButtonID];
    if(backButton.isPressed())
    {
        if(this->selectedButton == backButtonID)
        {
            backButton.action();
        }
        else
        {
            this->selectedButton = backButtonID;
        }
    }

    int otherButtonID = static_cast<int>(this->ready ? WAITING_FOR_PLAYERS_NOT_READY : WAITING_FOR_PLAYERS_READY);
    auto& otherButton = this->buttons[otherButtonID];
    if(otherButton.isPressed())
    {
        if(this->selectedButton == otherButtonID)
        {
            otherButton.action();
        }
        else
        {
            this->selectedButton = otherButtonID;
        }
    }

    if(kDown & KEY_A)
    {
        this->buttons[this->selectedButton].action();
    }
    else if(kDown & KEY_LEFT)
    {
        this->selectedButton = backButtonID;
    }
    else if(kDown & KEY_RIGHT)
    {
        this->selectedButton = otherButtonID;
    }
    else if(kDown & KEY_B)
    {
        this->buttons[backButtonID].action();
    }
}

void WaitingForPlayersState::draw()
{
    std::vector<std::string> playerNames = this->room->getPlayerNames();
    static float textScale = 0.6f;

    C2D_Text readyText, roomText;
    
    float readyTextWidth = 0;
    C2D_TextParse(&readyText, dynamicBuf, "Ready");
    C2D_TextOptimize(&readyText);
    C2D_TextGetDimensions(&readyText, textScale, textScale, &readyTextWidth, nullptr);
    float readyX = 208-readyTextWidth;

    for(size_t i = 0; i < playerNames.size(); i++)
    {
        float y = 40 + i*25;

        C2D_Text nameText;
        C2D_TextParse(&nameText, dynamicBuf, playerNames[i].c_str());
        C2D_TextOptimize(&nameText);
        C2D_DrawText(&nameText, C2D_WithColor, 32, y, 0.5f, textScale, textScale, textColor);

        if(this->room->isPlayerReady(i))
            C2D_DrawText(&readyText, C2D_WithColor, readyX, y, 0.5f, textScale, textScale, textColor);
    }

    int backButtonID = static_cast<int>(WAITING_FOR_PLAYERS_BACK);
    auto& backButton = this->buttons[backButtonID];
    backButton.draw();
    if(this->selectedButton == backButtonID)
        backButton.drawOverlay();

    int otherButtonID = static_cast<int>(this->ready ? WAITING_FOR_PLAYERS_NOT_READY : WAITING_FOR_PLAYERS_READY);
    auto& otherButton = this->buttons[otherButtonID];
    otherButton.draw();
    if(this->selectedButton == otherButtonID)
        otherButton.drawOverlay();
}

void WaitingForPlayersState::setReady()
{
    this->ready = true;
    int backButtonID = static_cast<int>(WAITING_FOR_PLAYERS_BACK);
    int waitingButtonID = static_cast<int>(WAITING_FOR_PLAYERS_NOT_READY);
    if(this->selectedButton != backButtonID)
        this->selectedButton = waitingButtonID;
}

void WaitingForPlayersState::setNotReady()
{
    this->ready = false;
    int backButtonID = static_cast<int>(WAITING_FOR_PLAYERS_BACK);
    int readyButtonID = static_cast<int>(WAITING_FOR_PLAYERS_READY);
    if(this->selectedButton != backButtonID)
        this->selectedButton = readyButtonID;
}

void WaitingForPlayersState::goBack()
{
    this->nextState = new NetworkMultiplayerState;
}
