#include "game/drawing.h"

void drawTopBanner()
{
    C2D_DrawImageAt(C2D_SpriteSheetGetImage(spritesheet, sprites_banner_idx), 96, 56, 0.5f);
    C2D_Text version;
    C2D_TextParse(&version, dynamicBuf, VERSION);
    C2D_TextOptimize(&version);
    static float textScale = 0.5f;
    float width = 0;
    C2D_TextGetDimensions(&version, textScale, textScale, &width, nullptr);
    float x = 286 - width;
    C2D_DrawText(&version, C2D_WithColor, x, 76, 0.5f, textScale, textScale, blackColor);
}

void drawBottomField()
{
    C2D_DrawRectSolid(16, 16, 0.1f, 320-32, 240-32, fieldColor);

    int start = 32;
    for(int i = 0; i < 8; i++)
    {
        C2D_DrawImageAt(C2D_SpriteSheetGetImage(spritesheet, sprites_border_left_right_idx), start+(i*32), 0, 0.2f, nullptr, 1.0f, -1.0f);
        C2D_DrawImageAt(C2D_SpriteSheetGetImage(spritesheet, sprites_border_left_right_idx), start+(i*32), 240-start, 0.2f, nullptr, 1.0f, 1.0f);
    }
    for(int i = 0; i < 6; i++)
    {
        C2D_DrawImageAt(C2D_SpriteSheetGetImage(spritesheet, sprites_border_top_down_idx), 0, start+i*32, 0.2f, nullptr, -1.0f, 1.0f);
        C2D_DrawImageAt(C2D_SpriteSheetGetImage(spritesheet, sprites_border_top_down_idx), 320-start, start+i*32, 0.2f, nullptr, 1.0f, 1.0f);
    }

    C2D_DrawImageAt(C2D_SpriteSheetGetImage(spritesheet, sprites_corner_idx), 0, 0, 0.3f, nullptr, -1.0f, -1.0f);
    C2D_DrawImageAt(C2D_SpriteSheetGetImage(spritesheet, sprites_corner_idx), 320-start, 0, 0.3f, nullptr, 1.0f, -1.0f);
    C2D_DrawImageAt(C2D_SpriteSheetGetImage(spritesheet, sprites_corner_idx), 0, 240-start, 0.3f, nullptr, -1.0f, 1.0f);
    C2D_DrawImageAt(C2D_SpriteSheetGetImage(spritesheet, sprites_corner_idx), 320-start, 240-start, 0.3f, nullptr, 1.0f, 1.0f);
}
