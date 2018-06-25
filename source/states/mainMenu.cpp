#include "states/mainMenu.h"
#include "game/drawing.h"

static constexpr float MainMenuButtonsX = 40;
static constexpr float MainMenuButtonsY[MAINMENU_BUTTONS_AMOUNT] = {
    25,
    75,
    125,
    175,
};

static constexpr int MainMenuMinSelected = 0;
static constexpr int MainMenuMaxSelected = static_cast<int>(MAINMENU_BUTTONS_AMOUNT)-1;

MainMenuState::MainMenuState()
{
    DEBUG("MainMenuState::MainMenuState\n");
    this->buttons[MAINMENU_BUTTONS_SINGLEPLAYER] = Button(MainMenuButtonsX, MainMenuButtonsY[MAINMENU_BUTTONS_SINGLEPLAYER], 0.5f, "Singleplayer", std::bind(&MainMenuState::goToSingleplayerMenu, this));
    this->buttons[MAINMENU_BUTTONS_MULTIPLAYER] = Button(MainMenuButtonsX, MainMenuButtonsY[MAINMENU_BUTTONS_MULTIPLAYER], 0.5f, "Multiplayer", std::bind(&MainMenuState::goToMultiplayerMenu, this));
    this->buttons[MAINMENU_BUTTONS_RULES] = Button(MainMenuButtonsX, MainMenuButtonsY[MAINMENU_BUTTONS_RULES], 0.5f, "Rules", std::bind(&MainMenuState::goToRulesMenu, this));
    this->buttons[MAINMENU_BUTTONS_QUIT] = Button(MainMenuButtonsX, MainMenuButtonsY[MAINMENU_BUTTONS_QUIT], 0.5f, "Quit", std::bind(&MainMenuState::quit, this));

    this->selectedButton = MainMenuMinSelected;
}

MainMenuState::~MainMenuState()
{
    DEBUG("MainMenuState::~MainMenuState\n");
}

void MainMenuState::update()
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
        if(this->selectedButton < MainMenuMinSelected)
            this->selectedButton = MainMenuMaxSelected;
    }
    else if(kDown & KEY_DOWN)
    {
        this->selectedButton++;
        if(this->selectedButton > MainMenuMaxSelected)
            this->selectedButton = MainMenuMinSelected;
    }
    else if(kDown & KEY_LEFT)
    {
        this->selectedButton = MainMenuMinSelected;
    }
    else if(kDown & KEY_RIGHT)
    {
        this->selectedButton = MainMenuMaxSelected;
    }
}

void MainMenuState::draw()
{
    for(auto& button : this->buttons)
    {
        button.draw();
    }
    C2D_DrawImageAt(C2D_SpriteSheetGetImage(spritesheet, sprites_button_overlay_idx), MainMenuButtonsX, MainMenuButtonsY[this->selectedButton], 0.6f);
}

void MainMenuState::goToSingleplayerMenu()
{
    this->nextState = new PlayingState(1, nullptr, 0);
}

void MainMenuState::goToMultiplayerMenu()
{
    this->nextState = new MultiplayerState;
}

void MainMenuState::goToRulesMenu()
{
    // this->nextState = new RulesState;
}

void MainMenuState::quit()
{
    running = false;
}
