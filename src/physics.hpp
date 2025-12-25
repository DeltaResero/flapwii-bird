// src/physics.hpp
// SPDX-License-Identifier: GPL-3.0-or-later
//
// Flapwii Bird
// Copyright (C) 2021-2025 TheBlueOompaLoompa
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

#pragma once

#include "pipe.hpp"
#include "vec2.hpp"

class Physics
{
private:
  const float gravity = 0.5;
  const float flap_height = -6.5;

  Vec2 position;

  void reset();
  bool is_colliding(Pipe pipe_1, Pipe pipe_2);
  void update_score(Pipe pipe_1, Pipe pipe_2);

public:
  Physics();
  ~Physics();

  Vec2 update_bird(bool flap, Pipe pipe_1, Pipe pipe_2);

  // Add getters for encapsulation
  Vec2 get_position() const
  {
    return position;
  }
  float get_x() const
  {
    return position.x;
  }
  float get_y() const
  {
    return position.y;
  }

  float velocity;
  bool pipe_iter = false;
  bool dead = false;
  int score = 0;
};

// EOF
