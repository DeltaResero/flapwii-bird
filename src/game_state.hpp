// src/game_state.hpp
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

#include "physics.hpp"
#include "pipe.hpp"
#include "audio.hpp"
#include <grrlib.h>
#include <wiiuse/wpad.h>
#include <memory>

class GameState
{
private:
  Pipe pipe_1;
  Pipe pipe_2;
  Physics physics;

  // Audio System
  std::unique_ptr<Audio> audio;

  // Cache bird position to avoid running physics in render
  Vec2 bird_position;

  bool first_round;
  bool is_menu;
  bool is_dying;

  // Cursor position for menu
  int cursor_x;
  int cursor_y;

  // Ground scroll offset for parallax effect
  float ground_scroll_offset;

  // Continuous scroll tracker
  float world_scroll_x;

  int score;
  int highscore;
  int last_score;

  char score_text[32];
  char highscore_text[32];

  void update_game(u32 buttons);
  void update_menu(u32 buttons, const ir_t &ir);
  void update_death_fall(u32 buttons);
  void handle_collision();
  void update_score_text();

  // Render helpers
  void render_menu(GRRLIB_texImg* bird_tex, GRRLIB_ttfFont* title_font);
  void render_game(GRRLIB_texImg* bird_tex, GRRLIB_texImg* pipe_tex);
  void render_pipe(GRRLIB_texImg* pipe_tex, const Pipe& pipe);
  void render_bird(GRRLIB_texImg* bird_tex, float x, float y, float rotation);
  void render_score(GRRLIB_ttfFont* font);
  void render_ground();

public:
  GameState();
  ~GameState();

  void update(u32 buttons, const ir_t &ir);
  void render(GRRLIB_texImg* bird_tex, GRRLIB_texImg* pipe_tex,
              GRRLIB_ttfFont* font, GRRLIB_ttfFont* title_font);

  void load_highscore();
  void save_highscore();
};

// EOF
