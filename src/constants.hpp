// src/constants.hpp
// SPDX-License-Identifier: GPL-3.0-or-later
//
// Flapwii Bird
// Copyright (C) 2025 DeltaResero
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

#pragma once

#include <gccore.h>

// Screen dimensions
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Bird constants
const int BIRD_WIDTH = 144;
const int BIRD_HEIGHT = 100;
const float BIRD_SCALE = 0.3f;
const float BIRD_START_X = SCREEN_WIDTH / 3.0f;
const float BIRD_START_Y = SCREEN_HEIGHT / 3.0f;

// Pipe constants
const int PIPE_WIDTH = 52;
const int PIPE_GAP = 100;
const float PIPE_SPEED = 1.0f;

// Wiimote constants
const int WSP_POINTER_CORRECTION_Y = 200;
const double WIIMOTE_SENSITIVITY = 0.7;

// Colors
const u32 GRRLIB_BLACK = 0x000000FF;
const u32 GRRLIB_WHITE = 0xFFFFFFFF;

// EOF
