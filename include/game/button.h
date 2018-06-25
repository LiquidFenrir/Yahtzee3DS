#pragma once

#include "common.h"

typedef void (*ButtonCallback)();

class Button
{
    public:
        Button() { };
        Button(float x, float y, float z, const std::string& text, ButtonCallback callback);

        bool isPressed();
        void draw();
        ButtonCallback action;

    private:
        float x, y, z;
        std::string text;
};
