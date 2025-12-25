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
  Physics::position.y = BIRD_START_X;
  Physics::position.x = BIRD_START_Y;
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
  Physics::position.y = BIRD_START_X;
  Physics::velocity = 0;
  Physics::score = 0;
  Physics::pipe_iter = false;
}

bool Physics::is_colliding(Pipe pipe_1, Pipe pipe_2)
{
  return
      // Pipe 1
      ((pipe_1.x - PIPE_WIDTH <= Physics::position.x + (BIRD_WIDTH * BIRD_SCALE) &&
        pipe_1.x >= Physics::position.x) &&
       (pipe_1.y <= Physics::position.y + (BIRD_HEIGHT * BIRD_SCALE) ||
        pipe_1.y - PIPE_GAP >= Physics::position.y)) ||

      // Pipe 2
      ((pipe_2.x - PIPE_WIDTH <= Physics::position.x + (BIRD_WIDTH * BIRD_SCALE) &&
        pipe_2.x >= Physics::position.x) &&
       (pipe_2.y <= Physics::position.y + (BIRD_HEIGHT * BIRD_SCALE) ||
        pipe_2.y - PIPE_GAP >= Physics::position.y)) ||

      // Screen top bottom
      Physics::position.y > SCREEN_HEIGHT || Physics::position.y < 0;
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
