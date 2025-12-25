// src/game_state.cpp
// SPDX-License-Identifier: GPL-3.0-or-later
//
// Flapwii Bird
// Copyright (C) 2025 DeltaResero
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

#include "game_state.hpp"
#include "constants.hpp"
#include <fat.h>
#include <fstream>
#include <stdio.h>

GameState::GameState()
  : first_round(true)
  , is_menu(true)
  , cursor_x(0)
  , cursor_y(0)
  , score(0)
  , highscore(0)
{
  bird_position.x = BIRD_START_X;
  bird_position.y = BIRD_START_Y;
  load_highscore();
}

GameState::~GameState()
{
  save_highscore();
}

void GameState::update(u32 buttons, const ir_t &ir)
{
  if (is_menu)
  {
    update_menu(buttons, ir);
  }
  else
  {
    update_game(buttons);
  }
}

void GameState::update_menu(u32 buttons, const ir_t &ir)
{
  cursor_x = ir.sx * WIIMOTE_SENSITIVITY;
  cursor_y = (ir.sy - WSP_POINTER_CORRECTION_Y) * WIIMOTE_SENSITIVITY;

  if (buttons & WPAD_BUTTON_A)
  {
    is_menu = false;
  }
}

void GameState::update_game(u32 buttons)
{
  bird_position = physics.update_bird(buttons & WPAD_BUTTON_A, pipe_1, pipe_2);
  score = physics.score;

  // Update pipes
  pipe_1.move();
  if (pipe_1.x < SCREEN_WIDTH / 2 && first_round)
  {
    first_round = false;
  }
  if (pipe_1.x < -PIPE_WIDTH)
  {
    pipe_1.reset();
  }

  if (!first_round)
  {
    pipe_2.move();
    if (pipe_2.x < -PIPE_WIDTH)
    {
      pipe_2.reset();
    }
  }

  // Handle collision
  if (physics.dead)
  {
    handle_collision();
  }

  // Update highscore
  if (score > highscore)
  {
    highscore = score;
  }

  update_score_text();
}

void GameState::handle_collision()
{
  first_round = true;
  pipe_1.reset();
  pipe_2.reset();
  score = 0;
  is_menu = true;
}

void GameState::update_score_text()
{
  sprintf(score_text, "Score: %i", score);
  sprintf(highscore_text, "Highscore: %i", highscore);
}

void GameState::render(GRRLIB_texImg* bird_tex, GRRLIB_texImg* pipe_tex,
                       GRRLIB_ttfFont* font, GRRLIB_ttfFont* title_font)
{
  if (is_menu)
  {
    render_menu(bird_tex, title_font);
  }
  else
  {
    render_game(bird_tex, pipe_tex);
  }

  render_score(font);
}

void GameState::render_menu(GRRLIB_texImg* bird_tex, GRRLIB_ttfFont* title_font)
{
  GRRLIB_PrintfTTF(165, 70, title_font, "Flapwii Bird", 96, 0xf6ef29ff);
  GRRLIB_PrintfTTF(175, 300, title_font, "Press A to flap", 72, 0xf6ef29ff);
  GRRLIB_DrawImg(cursor_x, cursor_y, bird_tex, 0, 1, 1, GRRLIB_WHITE);
}

void GameState::render_pipe(GRRLIB_texImg* pipe_tex, const Pipe& pipe)
{
  // Bottom pipe
  GRRLIB_DrawImg(pipe.x, pipe.y, pipe_tex, 0, 1, 1, GRRLIB_WHITE);
  // Top pipe (flipped)
  GRRLIB_DrawImg(pipe.x, pipe.y - PIPE_GAP, pipe_tex, 180, -1, 1, GRRLIB_WHITE);
}

void GameState::render_bird(GRRLIB_texImg* bird_tex, float x, float y, float rotation)
{
  GRRLIB_DrawImg(x, y, bird_tex, rotation, BIRD_SCALE, BIRD_SCALE, GRRLIB_WHITE);
}

void GameState::render_score(GRRLIB_ttfFont* font)
{
  GRRLIB_PrintfTTF(20, 10, font, score_text, 24, 0xf6ef23ff);
  GRRLIB_PrintfTTF(150, 10, font, highscore_text, 24, 0xf6ef23ff);
}

void GameState::render_game(GRRLIB_texImg* bird_tex, GRRLIB_texImg* pipe_tex)
{
  // Render first pipe
  render_pipe(pipe_tex, pipe_1);

  // Render second pipe if active
  if (!first_round)
  {
    render_pipe(pipe_tex, pipe_2);
  }

  // Render bird
  Vec2 bird_pos = physics.get_position();
  float bird_rotation = physics.velocity * 1.3f;
  render_bird(bird_tex, BIRD_START_X, bird_pos.y, bird_rotation);
}

void GameState::load_highscore()
{
  fatInitDefault();
  std::ifstream save;
  save.open("/apps/flapwii/game.sav", std::ifstream::in);

  if (!save.fail())
  {
    save >> highscore;
  }
  save.close();
}

void GameState::save_highscore()
{
  std::ofstream save;
  save.open("/apps/flapwii/game.sav");
  save << highscore;
  save.close();
}

// EOF
