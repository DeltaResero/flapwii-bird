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
  Physics::velocity = 0;
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

Hitbox Physics::get_bird_hitbox() const
{
  return Hitbox(
    position.x,
    position.y,
    BIRD_WIDTH * BIRD_SCALE,
    BIRD_HEIGHT * BIRD_SCALE
  );
}

Hitbox Physics::get_pipe_top_hitbox(const Pipe& pipe) const
{
  // Top pipe goes from y=0 to pipe.y - PIPE_GAP
  return Hitbox(
    pipe.x,
    0,
    PIPE_WIDTH,
    pipe.y - PIPE_GAP
  );
}

Hitbox Physics::get_pipe_bottom_hitbox(const Pipe& pipe) const
{
  // Bottom pipe starts at pipe.y and extends down
  return Hitbox(
    pipe.x,
    pipe.y,
    PIPE_WIDTH,
    SCREEN_HEIGHT - pipe.y
  );
}

bool Physics::is_colliding(Pipe pipe_1, Pipe pipe_2)
{
  Hitbox bird = get_bird_hitbox();

  // Check collision with pipe 1
  if (bird.intersects(get_pipe_top_hitbox(pipe_1)) ||
      bird.intersects(get_pipe_bottom_hitbox(pipe_1)))
  {
    return true;
  }

  // Check collision with pipe 2
  if (bird.intersects(get_pipe_top_hitbox(pipe_2)) ||
      bird.intersects(get_pipe_bottom_hitbox(pipe_2)))
  {
    return true;
  }

  // Check screen bounds (only top and bottom matter)
  if (bird.top() < 0 || bird.bottom() > SCREEN_HEIGHT)
  {
    return true;
  }

  return false;
}

void Physics::update_score(Pipe pipe_1, Pipe pipe_2)
{
  const float bird_center_x = position.x + (BIRD_WIDTH * BIRD_SCALE) / 2;
  const float scoring_zone = 20; // Tolerance

  // Check if bird's center just passed the pipe's trailing edge
  bool pipe1_passed = !pipe_iter &&
                      pipe_1.x + PIPE_WIDTH < bird_center_x &&
                      pipe_1.x + PIPE_WIDTH + scoring_zone > bird_center_x;

  bool pipe2_passed = pipe_iter &&
                      pipe_2.x + PIPE_WIDTH < bird_center_x &&
                      pipe_2.x + PIPE_WIDTH + scoring_zone > bird_center_x;

  if (pipe1_passed || pipe2_passed)
  {
    pipe_iter = !pipe_iter;
    score++;
  }
}

// EOF
