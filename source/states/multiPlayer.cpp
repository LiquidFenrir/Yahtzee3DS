#include "states/multiPlayer.h"
#include "game/drawing.h"

static constexpr float MultiplayerButtonsX = 60;
static constexpr float MultiplayerButtonsTextureWidth = 200;
static constexpr float MultiplayerButtonsZ = 0.5f;
static constexpr float MultiplayerButtonsTextOffset = 0;
static constexpr float MultiplayerButtonsY[MULTIPLAYER_BUTTONS_AMOUNT] = {
    40,
    100,
    160
};

static constexpr int MultiplayerMinSelected = 0;
static constexpr int MultiplayerMaxSelected = static_cast<int>(MULTIPLAYER_BUTTONS_AMOUNT)-1;

MultiplayerState::MultiplayerState()
{
    DEBUG("MultiplayerState::MultiplayerState\n");
    this->buttons[MULTIPLAYER_BUTTONS_LOCAL] = Button(MultiplayerButtonsX, MultiplayerButtonsY[MULTIPLAYER_BUTTONS_LOCAL], MultiplayerButtonsZ, MultiplayerButtonsTextureWidth, MultiplayerButtonsTextOffset, "Single console", std::bind(&MultiplayerState::goToLocalMenu, this));
    this->buttons[MULTIPLAYER_BUTTONS_NETWORK] = Button(MultiplayerButtonsX, MultiplayerButtonsY[MULTIPLAYER_BUTTONS_NETWORK], MultiplayerButtonsZ, MultiplayerButtonsTextureWidth, MultiplayerButtonsTextOffset, "Multiple consoles", std::bind(&MultiplayerState::goToNetworkMenu, this));
    this->buttons[MULTIPLAYER_BUTTONS_BACK] = Button(MultiplayerButtonsX, MultiplayerButtonsY[MULTIPLAYER_BUTTONS_BACK], MultiplayerButtonsZ, MultiplayerButtonsTextureWidth, MultiplayerButtonsTextOffset, "Back", std::bind(&MultiplayerState::goBack, this));
    this->selectedButton = MultiplayerMinSelected;
}

MultiplayerState::~MultiplayerState()
{
    DEBUG("MultiplayerState::~MultiplayerState\n");
}

void MultiplayerState::update()
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
        if(this->selectedButton < MultiplayerMinSelected)
            this->selectedButton = MultiplayerMaxSelected;
    }
    else if(kDown & KEY_DOWN)
    {
        this->selectedButton++;
        if(this->selectedButton > MultiplayerMaxSelected)
            this->selectedButton = MultiplayerMinSelected;
    }
    else if(kDown & KEY_LEFT)
    {
        this->selectedButton = MultiplayerMinSelected;
    }
    else if(kDown & KEY_RIGHT)
    {
        this->selectedButton = MultiplayerMaxSelected;
    }
    else if(kDown & KEY_B)
    {
        this->buttons[MULTIPLAYER_BUTTONS_BACK].action();
    }
}

void MultiplayerState::draw()
{
    for(size_t i = 0; i < this->buttons.size(); i++)
    {
        this->buttons[i].draw();
        if(i == static_cast<size_t>(this->selectedButton))
            this->buttons[i].drawOverlay();
    }
}

void MultiplayerState::goToLocalMenu()
{
    this->nextState = new LocalMultiplayerState;
}

void MultiplayerState::goToNetworkMenu()
{
    this->nextState = new NetworkMultiplayerState;
}

void MultiplayerState::goBack()
{
    this->nextState = new MainMenuState;
}
