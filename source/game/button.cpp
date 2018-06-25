#include "game/button.h"
#include "game/drawing.h"

Button::Button(float x, float y, float z, const std::string& text, ButtonCallback callback)
{
    DEBUG("Button::Button\n");
    this->x = x;
    this->y = y;
    this->z = z;
    this->text = text;
    this->action = callback;
}

void Button::draw()
{
    C2D_Text text;
    C2D_TextParse(&text, dynamicBuf, this->text.c_str());
    C2D_TextOptimize(&text);

    float width = 0;
    C2D_TextGetDimensions(&text, 1.0f, 1.0f, &width, NULL);
    float offset = 320/2 - width/2;

    C2D_DrawImageAt(C2D_SpriteSheetGetImage(spritesheet, sprites_button_idx), this->x, this->y, this->z);
    C2D_DrawText(&text, C2D_WithColor, offset, this->y+4, this->z+0.1f, 1.0f, 1.0f, textColor);
}

bool Button::isPressed()
{
    return kDown & KEY_TOUCH && (touch.px >= this->x && touch.px <= this->x+240 && touch.py >= this->y && touch.py <= this->y+40);
}
