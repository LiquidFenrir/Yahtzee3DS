#pragma once

#include "states/states.h"

class LocalMultiplayerState : public State
{
    public:
        LocalMultiplayerState();
        ~LocalMultiplayerState();

        void update();
        void draw();

    private:
        int playersAmount;
        u64 diceTouched;
};
