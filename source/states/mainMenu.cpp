#include "states/mainMenu.h"
#include "game/drawing.h"

static constexpr float MainMenuButtonsX = 60;
static constexpr float MainMenuButtonsTextureWidth = 200;
static constexpr float MainMenuButtonsZ = 0.5f;
static constexpr float MainMenuButtonsTextOffset = 0;
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
    this->buttons[MAINMENU_BUTTONS_SINGLEPLAYER] = Button(MainMenuButtonsX, MainMenuButtonsY[MAINMENU_BUTTONS_SINGLEPLAYER], MainMenuButtonsZ, MainMenuButtonsTextureWidth, MainMenuButtonsTextOffset, "Singleplayer", std::bind(&MainMenuState::goToSingleplayerMenu, this));
    this->buttons[MAINMENU_BUTTONS_MULTIPLAYER] = Button(MainMenuButtonsX, MainMenuButtonsY[MAINMENU_BUTTONS_MULTIPLAYER], MainMenuButtonsZ, MainMenuButtonsTextureWidth, MainMenuButtonsTextOffset, "Multiplayer", std::bind(&MainMenuState::goToMultiplayerMenu, this));
    this->buttons[MAINMENU_BUTTONS_RULES] = Button(MainMenuButtonsX, MainMenuButtonsY[MAINMENU_BUTTONS_RULES], MainMenuButtonsZ, MainMenuButtonsTextureWidth, MainMenuButtonsTextOffset, "Rules", std::bind(&MainMenuState::goToRulesMenu, this));
    this->buttons[MAINMENU_BUTTONS_QUIT] = Button(MainMenuButtonsX, MainMenuButtonsY[MAINMENU_BUTTONS_QUIT], MainMenuButtonsZ, MainMenuButtonsTextureWidth, MainMenuButtonsTextOffset, "Quit", std::bind(&MainMenuState::quit, this));

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
    for(size_t i = 0; i < this->buttons.size(); i++)
    {
        this->buttons[i].draw();
        if(i == static_cast<size_t>(this->selectedButton))
            this->buttons[i].drawOverlay();
    }
}

void MainMenuState::goToSingleplayerMenu()
{
    this->nextState = new PlayingState(1, 0);
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
