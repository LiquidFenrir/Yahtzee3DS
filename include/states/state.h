#pragma once

#include "common.h"

class State
{
    public:
        virtual ~State() { };

        virtual void update();
        virtual void draw();

        State* nextState  = nullptr;
};
