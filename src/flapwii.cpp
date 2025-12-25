// src/flapwii.cpp
// SPDX-License-Identifier: GPL-3.0-or-later
//
// Flapwii Bird
// Copyright (C) 2021-2025 TheBlueOompaLoompa
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

#include <gccore.h>
#include <wiiuse/wpad.h>

#include "constants.hpp"
#include "game_state.hpp"

// GRRLIB
#include <grrlib.h>

#include "bird_png.h"
#include "flappy_ttf.h"
#include "font_ttf.h"
#include "pipe_png.h"

int main(void)
{
  GRRLIB_Init();

  GRRLIB_texImg* bird = GRRLIB_LoadTexture(bird_png);
  GRRLIB_texImg* pipe = GRRLIB_LoadTexture(pipe_png);

  GRRLIB_ttfFont* font = GRRLIB_LoadTTF(font_ttf, font_ttf_size);
  GRRLIB_ttfFont* flappy_font = GRRLIB_LoadTTF(flappy_ttf, flappy_ttf_size);

  WPAD_Init();
  WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS_ACC_IR);

  GameState game;
  ir_t ir;

  while (1)
  {
    GRRLIB_FillScreen(0x0195c3ff);
    WPAD_ScanPads();
    WPAD_IR(WPAD_CHAN_0, &ir);

    u32 buttons = WPAD_ButtonsDown(WPAD_CHAN_0);

    if (buttons & WPAD_BUTTON_HOME)
    {
      break;
    }

    game.update(buttons, ir);
    game.render(bird, pipe, font, flappy_font);

    GRRLIB_Render();
  }

  // Cleanup
  GRRLIB_FreeTTF(font);
  GRRLIB_FreeTTF(flappy_font);
  GRRLIB_FreeTexture(bird);
  GRRLIB_FreeTexture(pipe);
  GRRLIB_Exit();

  return 0;
}

// EOF
