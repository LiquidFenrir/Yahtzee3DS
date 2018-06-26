#include "states/multiPlayer.h"
#include "game/drawing.h"

static constexpr float NetworkMultiplayerButtonsX = 60;
static constexpr float NetworkMultiplayerButtonsTextureWidth = 200;
static constexpr float NetworkMultiplayerButtonsZ = 0.5f;
static constexpr float NetworkMultiplayerButtonsTextOffset = 0;
static constexpr float NetworkMultiplayerButtonsY[MULTIPLAYER_BUTTONS_AMOUNT] = {
    40,
    100,
    160
};

static constexpr int NetworkMultiplayerMinSelected = 0;
static constexpr int NetworkMultiplayerMaxSelected = static_cast<int>(NETWORK_MULTIPLAYER_BUTTONS_AMOUNT)-1;

NetworkMultiplayerState::NetworkMultiplayerState()
{
    DEBUG("NetworkMultiplayerState::NetworkMultiplayerState\n");
    this->buttons[NETWORK_MULTIPLAYER_BUTTONS_CREATE_ROOM] = Button(NetworkMultiplayerButtonsX, NetworkMultiplayerButtonsY[MULTIPLAYER_BUTTONS_LOCAL], NetworkMultiplayerButtonsZ, NetworkMultiplayerButtonsTextureWidth, NetworkMultiplayerButtonsTextOffset, "Create Room", std::bind(&NetworkMultiplayerState::createRoom, this));
    this->buttons[NETWORK_MULTIPLAYER_BUTTONS_LIST_ROOMS] = Button(NetworkMultiplayerButtonsX, NetworkMultiplayerButtonsY[MULTIPLAYER_BUTTONS_NETWORK], NetworkMultiplayerButtonsZ, NetworkMultiplayerButtonsTextureWidth, NetworkMultiplayerButtonsTextOffset, "List Rooms", std::bind(&NetworkMultiplayerState::listRooms, this));
    this->buttons[NETWORK_MULTIPLAYER_BUTTONS_BACK] = Button(NetworkMultiplayerButtonsX, NetworkMultiplayerButtonsY[MULTIPLAYER_BUTTONS_BACK], NetworkMultiplayerButtonsZ, NetworkMultiplayerButtonsTextureWidth, NetworkMultiplayerButtonsTextOffset, "Back", std::bind(&NetworkMultiplayerState::goBack, this));
    this->selectedButton = NetworkMultiplayerMinSelected;
}

NetworkMultiplayerState::~NetworkMultiplayerState()
{
    DEBUG("NetworkMultiplayerState::~NetworkMultiplayerState\n");
}

void NetworkMultiplayerState::update()
{
    for(size_t i = 0; i < this->buttons.size(); i++)
    {
        auto& button = this->buttons[i];
        if(button.isPressed())
        {
            if(i == static_cast<size_t>(this->selectedButton))
            {
                button.action();
            }
            else
            {
                this->selectedButton = static_cast<int>(i);
            }
        }
    }

    if(kDown & KEY_A)
    {
        this->buttons[this->selectedButton].action();
    }
    else if(kDown & KEY_UP)
    {
        this->selectedButton--;
        if(this->selectedButton < NetworkMultiplayerMinSelected)
            this->selectedButton = NetworkMultiplayerMaxSelected;
    }
    else if(kDown & KEY_DOWN)
    {
        this->selectedButton++;
        if(this->selectedButton > NetworkMultiplayerMaxSelected)
            this->selectedButton = NetworkMultiplayerMinSelected;
    }
    else if(kDown & KEY_LEFT)
    {
        this->selectedButton = NetworkMultiplayerMinSelected;
    }
    else if(kDown & KEY_RIGHT)
    {
        this->selectedButton = NetworkMultiplayerMaxSelected;
    }
    else if(kDown & KEY_B)
    {
        this->buttons[MULTIPLAYER_BUTTONS_BACK].action();
    }
}

void NetworkMultiplayerState::draw()
{
    for(size_t i = 0; i < this->buttons.size(); i++)
    {
        this->buttons[i].draw();
        if(i == static_cast<size_t>(this->selectedButton))
            this->buttons[i].drawOverlay();
    }
}

void NetworkMultiplayerState::createRoom()
{
    this->nextState = new WaitingForPlayersState;
}

void NetworkMultiplayerState::listRooms()
{
    this->nextState = new NetworkListState;
}

void NetworkMultiplayerState::goBack()
{
    this->nextState = new MultiplayerState;
}
