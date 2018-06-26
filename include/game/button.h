#pragma once

#include "common.h"

class Button
{
    public:
        Button() { };
        Button(float x, float y, float z, float textureWidth, float centerTextOffset, const std::string& text, std::function<void()> callback);

        bool isPressed();
        void draw();
        void drawOverlay();
        std::function<void()> action;

    private:
        float x, y, z;
        float textureWidth;
        std::string text;
        float centerTextOffset;

        float width, height, offset;
};
