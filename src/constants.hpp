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

// Ground constants - using fractions for resolution independence
const float GROUND_HEIGHT_RATIO = 0.16f;  // ~1/6 of screen (similar to original)
const int GROUND_HEIGHT = static_cast<int>(SCREEN_HEIGHT * GROUND_HEIGHT_RATIO);
const int GROUND_Y = SCREEN_HEIGHT - GROUND_HEIGHT;
const float GROUND_SCROLL_SPEED = PIPE_SPEED;  // Matches pipe speed for parallax
const int GROUND_PATTERN_WIDTH = 24;  // Width of repeating pattern

// Ground colors - earthy tones like original
const unsigned int GROUND_BASE_COLOR = 0xDED895FF;     // Tan/beige base
const unsigned int GROUND_GRASS_COLOR = 0x5AC54FFF;    // Bright grass green
const unsigned int GROUND_DARK_GRASS = 0x4A9E3FFF;     // Darker grass shade
const unsigned int GROUND_OUTLINE = 0x000000FF;        // Black outline

// Wiimote constants
const int WSP_POINTER_CORRECTION_Y = 200;
const double WIIMOTE_SENSITIVITY = 0.7;

// Colors
const unsigned int GRRLIB_BLACK = 0x000000FF;
const unsigned int GRRLIB_WHITE = 0xFFFFFFFF;

// EOF
