// src/pipe.cpp
// SPDX-License-Identifier: GPL-3.0-or-later
//
// Flapwii Bird
// Copyright (C) 2021-2025 TheBlueOompaLoompa
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

#include "pipe.hpp"
#include "constants.hpp"

Pipe::Pipe()
{
  Pipe::y = rand() % (SCREEN_HEIGHT / 2) + 1 + (SCREEN_HEIGHT / 4);
  Pipe::x = SCREEN_WIDTH;
  Pipe::speed = PIPE_SPEED;
}

Pipe::~Pipe()
{
}

void Pipe::move()
{
  Pipe::x -= Pipe::speed;
}

void Pipe::reset()
{
  Pipe::y = rand() % (SCREEN_HEIGHT / 2) + 1 + (SCREEN_HEIGHT / 4);
  Pipe::x = SCREEN_WIDTH;
}

// EOF
