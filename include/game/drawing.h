#pragma once

#include "common.h"

constexpr u32 backgroundColor = C2D_Color32(0x20, 0x20, 0x20, 0xFF); // Some nice gray, taken from QRaken
constexpr u32 fieldColor = C2D_Color32(0x03, 0x7C, 0x13, 0xFF); // Green
constexpr u32 borderColor = C2D_Color32(0x74, 0x3F, 0x00, 0xFF); // Brown
constexpr u32 blackColor = C2D_Color32f(0,0,0,1); // Black
constexpr u32 whiteColor = C2D_Color32f(1,1,1,1); // White
constexpr u32 textColor = whiteColor;
constexpr u32 greyedOutTextColor = C2D_Color32f(0.8f,0.8f,0.8f,1);

void drawTopBanner();
void drawBottomField();
