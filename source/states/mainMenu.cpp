#include "states/mainMenu.h"
#include "game/drawing.h"

static float MainMenuButtonsX = 40;
static float MainMenuButtonsY[MAINMENU_BUTTONS_AMOUNT] = {
    25,
    75,
    125,
    175,
};

static int MainMenuMinSelected = 0;
static int MainMenuMaxSelected = static_cast<int>(MAINMENU_BUTTONS_AMOUNT)-1;

MainMenuState::MainMenuState()
{
    DEBUG("MainMenuState::MainMenuState\n");
    this->buttons[MAINMENU_BUTTONS_SINGLEPLAYER] = Button(MainMenuButtonsX, MainMenuButtonsY[MAINMENU_BUTTONS_SINGLEPLAYER], 0.5f, "Singleplayer", [](){ });
    this->buttons[MAINMENU_BUTTONS_MULTIPLAYER] = Button(MainMenuButtonsX, MainMenuButtonsY[MAINMENU_BUTTONS_MULTIPLAYER], 0.5f, "Multiplayer", [](){ });
    this->buttons[MAINMENU_BUTTONS_RULES] = Button(MainMenuButtonsX, MainMenuButtonsY[MAINMENU_BUTTONS_RULES], 0.5f, "Rules", [](){ });
    this->buttons[MAINMENU_BUTTONS_QUIT] = Button(MainMenuButtonsX, MainMenuButtonsY[MAINMENU_BUTTONS_QUIT], 0.5f, "Quit", [](){ running = false; });

    this->selectedButton = 0;
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
