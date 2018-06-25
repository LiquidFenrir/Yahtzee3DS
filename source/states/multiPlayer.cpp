#include "states/multiPlayer.h"
#include "game/drawing.h"

static float MultiplayerButtonsX = 40;
static float MultiplayerButtonsY[MULTIPLAYER_BUTTONS_AMOUNT] = {
    40,
    100,
    160
};

static int MultiplayerMinSelected = 0;
static int MultiplayerMaxSelected = static_cast<int>(MULTIPLAYER_BUTTONS_AMOUNT)-1;

MultiplayerState::MultiplayerState()
{
    DEBUG("MultiplayerState::MultiplayerState\n");
    this->buttons[MULTIPLAYER_BUTTONS_LOCAL] = Button(MultiplayerButtonsX, MultiplayerButtonsY[MULTIPLAYER_BUTTONS_LOCAL], 0.5f, "Single console", std::bind(&MultiplayerState::goToLocalMenu, this));
    this->buttons[MULTIPLAYER_BUTTONS_NETWORK] = Button(MultiplayerButtonsX, MultiplayerButtonsY[MULTIPLAYER_BUTTONS_NETWORK], 0.5f, "Multiple consoles", std::bind(&MultiplayerState::goToNetworkMenu, this));
    this->buttons[MULTIPLAYER_BUTTONS_BACK] = Button(MultiplayerButtonsX, MultiplayerButtonsY[MULTIPLAYER_BUTTONS_BACK], 0.5f, "Back", std::bind(&MultiplayerState::goBack, this));
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
    for(auto& button : this->buttons)
    {
        button.draw();
    }
    C2D_DrawImageAt(C2D_SpriteSheetGetImage(spritesheet, sprites_button_overlay_idx), MultiplayerButtonsX, MultiplayerButtonsY[this->selectedButton], 0.6f);
}

void MultiplayerState::goToLocalMenu()
{
    this->nextState = new LocalMultiplayerState;
}

void MultiplayerState::goToNetworkMenu()
{
    // this->nextState = new NetworkMultiplayerState;
}

void MultiplayerState::goBack()
{
    this->nextState = new MainMenuState;
}
