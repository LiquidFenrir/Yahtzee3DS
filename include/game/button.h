#pragma once

#include "common.h"

class Button
{
    public:
        Button() { };
        Button(float x, float y, float z, const std::string& text, std::function<void()> callback);

        bool isPressed();
        void draw();
        std::function<void()> action;

    private:
        float x, y, z;
        std::string text;
};
