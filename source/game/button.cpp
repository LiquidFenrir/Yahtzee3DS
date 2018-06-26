#include "game/button.h"
#include "game/drawing.h"

Button::Button(float x, float y, float z, float textureWidth, float centerTextOffset, const std::string& text, std::function<void()> callback)
{
    DEBUG("Button::Button\n");
    this->x = x;
    this->y = y;
    this->z = z;
    this->textureWidth = textureWidth;
    this->centerTextOffset = centerTextOffset;

    this->text = text;
    this->action = callback;

    this->width = 0;
    this->height = 0;
    this->offset = 0;
}

void Button::draw()
{
    static float textScale = 0.8f;

    C2D_Text text;
    C2D_TextParse(&text, dynamicBuf, this->text.c_str());
    C2D_TextOptimize(&text);

    if(!this->offset)
    {
        C2D_TextGetDimensions(&text, textScale, textScale, &this->width, &this->height);
        this->offset = this->centerTextOffset + (320/2 - this->width/2);
    }

    float endx = this->x+this->textureWidth-20;
    for(float x = this->x+20; x < endx; x += 20)
        C2D_DrawImageAt(C2D_SpriteSheetGetImage(spritesheet, sprites_button_body_idx), x, this->y, this->z);
    C2D_DrawImageAt(C2D_SpriteSheetGetImage(spritesheet, sprites_button_end_idx), this->x, this->y, this->z+0.01f);
    C2D_DrawImageAt(C2D_SpriteSheetGetImage(spritesheet, sprites_button_end_idx), endx, this->y, this->z+0.01f, nullptr, -1.0f, 1.0f);
    
    C2D_DrawText(&text, C2D_WithColor | C2D_AtBaseline, offset, this->y+this->height+2, this->z+0.02f, textScale, textScale, textColor);
}

void Button::drawOverlay()
{
    float endx = this->x+this->textureWidth-20;
    for(float x = this->x+20; x < endx; x += 20)
        C2D_DrawImageAt(C2D_SpriteSheetGetImage(spritesheet, sprites_button_overlay_body_idx), x, this->y, this->z+0.03f);
    C2D_DrawImageAt(C2D_SpriteSheetGetImage(spritesheet, sprites_button_overlay_end_idx), this->x, this->y, this->z+0.04f);
    C2D_DrawImageAt(C2D_SpriteSheetGetImage(spritesheet, sprites_button_overlay_end_idx), endx, this->y, this->z+0.04f, nullptr, -1.0f, 1.0f);
}

bool Button::isPressed()
{
    return kDown & KEY_TOUCH && (touch.px >= this->x && touch.px <= this->x+240 && touch.py >= this->y && touch.py <= this->y+40);
}
