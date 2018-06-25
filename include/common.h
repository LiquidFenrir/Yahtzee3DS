#pragma once

#include <memory>
#include <algorithm>
#include <functional>

#include <vector>
#include <array>
#include <tuple>
#include <list>

#include <cstdio>

#include <3ds.h>
#include <citro3d.h>
#include <citro2d.h>

#include "sprites.h"

#define DEBUG(...) fprintf(stderr, __VA_ARGS__)

extern bool running;

extern C2D_SpriteSheet spritesheet;

extern C3D_RenderTarget *top, *bottom;
extern C2D_TextBuf staticBuf, dynamicBuf;

extern u32 kDown, kHeld, kUp;
extern touchPosition touch;
