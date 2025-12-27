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

// C++ Standard Library
#include <fstream>

// C Standard Library
#include <stdio.h>

// System libraries
#include <fat.h>

// Project headers
#include "game_state.hpp"
#include "constants.hpp"

// ============================================================================
// Initialization & Cleanup
// ============================================================================

GameState::GameState()
  : first_round(true)
  , is_menu(true)
  , is_dying(false)
  , cursor_x(0)
  , cursor_y(0)
  , ground_scroll_offset(0)
  , world_scroll_x(0.0f)
  , score(0)
  , highscore(0)
  , last_score(0)
{
  // Initialize Audio System
  audio = std::make_unique<Audio>();

  bird_position.x = BIRD_START_X;
  bird_position.y = BIRD_START_Y;
  load_highscore();
  update_score_text();
}

GameState::~GameState()
{
  save_highscore();
}

// ============================================================================
// Game Logic Loop
// ============================================================================

void GameState::update(u32 buttons, const ir_t &ir)
{
  if (is_menu)
  {
    update_menu(buttons, ir);
  }
  else if (is_dying)
  {
    update_death_fall(buttons);
  }
  else
  {
    update_game(buttons);
  }
}

void GameState::update_menu(u32 buttons, const ir_t &ir)
{
  // Map Wiimote pointer to screen coordinates
  cursor_x = ir.sx * WIIMOTE_SENSITIVITY;
  cursor_y = (ir.sy - WSP_POINTER_CORRECTION_Y) * WIIMOTE_SENSITIVITY;

  if (buttons & WPAD_BUTTON_A)
  {
    audio->PlayTransition(); // Play transition sound
    is_menu = false;

    // Reset State
    ground_scroll_offset = 0;
    world_scroll_x = 0;       // Reset world coordinate seed
    physics.reset();          // Reset physics for fresh start

    // Reset Bird
    bird_position.x = BIRD_START_X;
    bird_position.y = BIRD_START_Y;
  }
}

void GameState::update_game(u32 buttons)
{
  bool did_flap = buttons & WPAD_BUTTON_A;
  bird_position = physics.update_bird(did_flap, pipe_1, pipe_2);

  if (did_flap && !physics.dead)
  {
    audio->PlayFlap();
  }

  score = physics.score;

  if (score > last_score)
  {
    audio->PlayScore();
    last_score = score;
  }

  // --------------------------------------------------------------------------
  // Pipe Management
  // --------------------------------------------------------------------------

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

  // --------------------------------------------------------------------------
  // World Scrolling
  // --------------------------------------------------------------------------

  // Parallax Grass: Wraps around the pattern width to save logic
  ground_scroll_offset -= GROUND_SCROLL_SPEED;
  if (ground_scroll_offset <= -GROUND_PATTERN_WIDTH)
  {
    ground_scroll_offset += GROUND_PATTERN_WIDTH;
  }

  // Procedural Dirt: Continually increases to provide a unique "seed"
  // for the noise generation, preventing the dirt texture from looping.
  world_scroll_x += GROUND_SCROLL_SPEED;

  // --------------------------------------------------------------------------
  // State Checks
  // --------------------------------------------------------------------------

  // Check if we just died this frame
  if (physics.dead && !is_dying)
  {
    audio->PlayHit(); // Always play hit sound on death

    // Only play the "fall" sound if we are NOT hitting the ground directly.
    // If we hit a pipe or the ceiling, we fall.
    // If we hit the ground, we just stop (no fall sound).
    if (bird_position.y + (BIRD_HEIGHT * BIRD_SCALE) < GROUND_Y)
    {
      audio->PlayFall();
    }

    is_dying = true;
  }

  // Update highscore
  if (score > highscore)
  {
    highscore = score;
  }

  update_score_text();
}

void GameState::update_death_fall(u32 buttons)
{
  // Continue physics simulation but ignore user input
  bird_position = physics.update_bird(false, pipe_1, pipe_2);

  // Check if bird hit the ground
  if (bird_position.y + (BIRD_HEIGHT * BIRD_SCALE) >= GROUND_Y)
  {
    // Do NOT play sound here.
    // If we fell from a pipe, sfx_fall played earlier.
    // If we hit the ground directly, sfx_hit played earlier.
    handle_collision();
  }
}

void GameState::handle_collision()
{
  first_round = true;
  is_dying = false;
  pipe_1.reset();
  pipe_2.reset();
  score = 0;
  last_score = 0;
  is_menu = true;
  ground_scroll_offset = 0;
  world_scroll_x = 0;
}

void GameState::update_score_text()
{
  sprintf(score_text, "Score: %i", score);
  sprintf(highscore_text, "Highscore: %i", highscore);
}

// ============================================================================
// Rendering
// ============================================================================

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

  // Draw the ground layer on top of pipes, unless in main menu
  if (!is_menu)
  {
    render_ground();
  }

  render_score(font);
}

void GameState::render_ground()
{
  // Configuration
  const int outline_height = 2;
  const int grass_height = 12;
  const int shadow_height = 2;
  const int chevron_spacing = 8;
  const int chevron_line_width = 4;

  // Colors (RGBA8 format)
  const unsigned int grass_background_dark = 0x4AAB3CFF; // Rich Forest Green
  const unsigned int chevron_color = 0x74D466FF;         // Bright Pastel Green

  // --------------------------------------------------------------------------
  // Layer: Top Outline
  // --------------------------------------------------------------------------

  GRRLIB_Rectangle(0, GROUND_Y - outline_height, SCREEN_WIDTH, outline_height,
                   GROUND_OUTLINE, true);

  // --------------------------------------------------------------------------
  // Layer: Grass
  // --------------------------------------------------------------------------

  // Background Fill:
  // We fill the entire grass strip with the darker green color first.
  GRRLIB_Rectangle(0, GROUND_Y, SCREEN_WIDTH, grass_height,
                   grass_background_dark, true);

  // Chevron Overlay:
  // We draw light green shapes ON TOP of the dark background.
  // This creates the ">>>>>" pattern effect.
  for (int x = static_cast<int>(ground_scroll_offset);
       x < SCREEN_WIDTH + chevron_spacing; x += chevron_spacing)
  {
    // Draw the upper diagonal stroke: /
    for (int i = 0; i < grass_height / 2; i++)
    {
      GRRLIB_Rectangle(x + i, GROUND_Y + i, chevron_line_width, 1, chevron_color, true);
    }
    // Draw the lower diagonal stroke: \ (flipped)
    for (int i = 0; i < grass_height / 2; i++)
    {
      GRRLIB_Rectangle(x + (grass_height / 2 - 1 - i), GROUND_Y + grass_height / 2 + i,
                       chevron_line_width, 1, chevron_color, true);
    }
  }

  // --------------------------------------------------------------------------
  // Layer: Shadow Divider
  // --------------------------------------------------------------------------

  GRRLIB_Rectangle(0, GROUND_Y + grass_height, SCREEN_WIDTH, shadow_height,
                   0x4A9E3FFF, true);

  // --------------------------------------------------------------------------
  // Layer: Dirt & Procedural Texture
  // --------------------------------------------------------------------------

  const int dirt_y = GROUND_Y + grass_height + shadow_height;
  const int dirt_height = SCREEN_HEIGHT - dirt_y;

  // Draw solid dirt base
  GRRLIB_Rectangle(0, dirt_y, SCREEN_WIDTH, dirt_height, GROUND_BASE_COLOR, true);

  // Procedural Noise Generation (Generate noise based on world coordinates)
  const unsigned int color_speck_dark = 0xB0A96CFF;  // Subtly darker brown
  const unsigned int color_speck_light = 0xF5F1BEFF; // Pale Chiffon/Cream
  const unsigned int color_rock = 0xB0A565FF;        // Earthy Metallic Brass

  int scroll_int = static_cast<int>(world_scroll_x);

  // Align to grid: We snap the starting position to a 4-pixel grid in
  // world-space so the hashing remains consistent while scrolling.
  int start_world_x = (scroll_int / 4) * 4;

  // Leave a 4px buffer to avoid drawing noise too close to edges
  for (int y = dirt_y + 4; y < SCREEN_HEIGHT - 4; y += 4)
  {
    // Iterate through WORLD coordinates that are currently visible on screen
    for (int w_x = start_world_x; w_x < start_world_x + SCREEN_WIDTH + 8; w_x += 4)
    {
      // Deterministic Hash:
      // This math guarantees that for any specific (x,y) in the world,
      // we always get the same random number.
      unsigned int h = ((w_x * 374761393U) ^ (y * 668265263U));
      h = (h ^ (h >> 13)) * 1274126177U;

      int val = (h >> 16) & 0xFF;    // Probability value (0-255)
      int offset_x = (h & 3);        // Jitter position X
      int offset_y = ((h >> 2) & 3); // Jitter position Y

      // Convert World Coordinate back to Screen Coordinate for drawing
      int draw_x = w_x - scroll_int + offset_x;
      int draw_y = y + offset_y;

      // Draw if within horizontal screen bounds
      if (draw_x >= 0 && draw_x < SCREEN_WIDTH)
      {
        if (val < 15)
        {
          GRRLIB_Plot(draw_x, draw_y, color_speck_dark);
        }
        else if (val < 20)
        {
          GRRLIB_Plot(draw_x, draw_y, color_speck_light);
        }
        else if (val == 25)
        {
          // Draw rock (checking bounds to prevent overflow)
          if (draw_x + 1 < SCREEN_WIDTH && draw_y + 1 < SCREEN_HEIGHT)
          {
            GRRLIB_Rectangle(draw_x, draw_y, 2, 2, color_rock, true);
          }
        }
      }
    }
  }
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
  // Top pipe (flipped vertically)
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
  render_bird(bird_tex, bird_pos.x, bird_pos.y, bird_rotation);
}

// ============================================================================
// Save Management (File I/O)
// ============================================================================

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
