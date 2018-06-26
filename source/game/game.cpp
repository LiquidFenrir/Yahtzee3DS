#include "game/game.h"
#include "game/drawing.h"

#include "states/states.h"

void deleteAllStates(State* state)
{
    if(state->nextState)
        deleteAllStates(state->nextState);
    delete state;
}

Game::Game(int argc, char* argv[])
{
    DEBUG("Game::Game\n");
    (void)argc;
    (void)argv;

    APT_GetAppCpuTimeLimit(&this->old_time_limit);
    APT_SetAppCpuTimeLimit(30);

    romfsInit();
    cfguInit();
    udsInit(0x3000, NULL);

    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();

    top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

    spritesheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");

    staticBuf = C2D_TextBufNew(4096);
    dynamicBuf = C2D_TextBufNew(4096);

    this->state = new MainMenuState;
}

Game::~Game()
{
    DEBUG("Game::~Game\n");

    deleteAllStates(this->state);

    C2D_TextBufDelete(dynamicBuf);
    C2D_TextBufDelete(staticBuf);

    C2D_Fini();
    C3D_Fini();
    gfxExit();

    udsExit();
    cfguExit();
    romfsExit();

    if(this->old_time_limit != UINT32_MAX)
    {
        APT_SetAppCpuTimeLimit(this->old_time_limit);
    }
}

void Game::update()
{   
    hidScanInput();

    kDown = hidKeysDown();
    kHeld = hidKeysHeld();
    kUp = hidKeysUp();

    hidTouchRead(&touch);

    if(kDown & KEY_START)
    {
        running = false;
        return;
    }

    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

    C2D_TextBufClear(dynamicBuf);

    C2D_SceneBegin(top);
    C2D_TargetClear(top, backgroundColor);

    drawTopBanner();

    C2D_SceneBegin(bottom);
    C2D_TargetClear(bottom, backgroundColor);

    drawBottomField();
    this->state->draw();

    C3D_FrameEnd(0);

    this->state->update();

    State* nextState = this->state->nextState;
    if(nextState)
    {
        DEBUG("nextState: %p\n", nextState);
        delete this->state;
        this->state = nextState;
    }
}
