// src/physics.cpp
// SPDX-License-Identifier: GPL-3.0-or-later
//
// Flapwii Bird
// Copyright (C) 2021-2025 TheBlueOompaLoompa
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

#include "physics.hpp"
#include "constants.hpp"

Physics::Physics()
{
  Physics::position.x = BIRD_START_X;
  Physics::position.y = BIRD_START_Y;
}
Physics::~Physics()
{
}

Vec2 Physics::update_bird(bool flap, Pipe pipe_1, Pipe pipe_2)
{
  if (flap)
  {
    velocity = Physics::flap_height;
  }
  else
  {
    velocity += Physics::gravity;
  }

  Physics::position.y += velocity;

  Physics::dead = is_colliding(pipe_1, pipe_2);

  Physics::update_score(pipe_1, pipe_2);

  if (Physics::dead)
  {
    reset();
  }

  return Physics::position;
}

void Physics::reset()
{
  Physics::position.x = BIRD_START_X;
  Physics::position.y = BIRD_START_Y;
  Physics::velocity = 0;
  Physics::score = 0;
  Physics::pipe_iter = false;
}

bool Physics::is_colliding(Pipe pipe_1, Pipe pipe_2)
{
  // Bird bounding box
  float bird_left = Physics::position.x;
  float bird_right = Physics::position.x + (BIRD_WIDTH * BIRD_SCALE);
  float bird_top = Physics::position.y;
  float bird_bottom = Physics::position.y + (BIRD_HEIGHT * BIRD_SCALE);

  // Helper to check collision with a single pipe
  auto check_pipe = [&](Pipe &p) -> bool
  {
    // Check horizontal overlap
    if (bird_right >= p.x && bird_left <= p.x + PIPE_WIDTH)
    {
      // In the horizontal danger zone. Safe only if inside the vertical gap.
      // Gap is between (p.y - PIPE_GAP) and p.y
      // Hit top pipe if bird_top < p.y - PIPE_GAP
      // Hit bottom pipe if bird_bottom > p.y
      if (bird_top < p.y - PIPE_GAP || bird_bottom > p.y)
      {
        return true;
      }
    }
    return false;
  };

  if (check_pipe(pipe_1) || check_pipe(pipe_2))
  {
    return true;
  }

  // Screen boundaries
  if (bird_top < 0 || bird_top > SCREEN_HEIGHT)
  {
    return true;
  }

  return false;
}

void Physics::update_score(Pipe pipe_1, Pipe pipe_2)
{
  if ((!Physics::pipe_iter && pipe_1.x + PIPE_WIDTH <= BIRD_START_X &&
       pipe_1.x + PIPE_WIDTH + 20 >= BIRD_START_X) ||
      (Physics::pipe_iter && pipe_2.x + PIPE_WIDTH <= BIRD_START_X &&
       pipe_2.x + PIPE_WIDTH + 20 >= BIRD_START_X))
  {
    Physics::pipe_iter = !Physics::pipe_iter;
    Physics::score++;
  }
}

// EOF
