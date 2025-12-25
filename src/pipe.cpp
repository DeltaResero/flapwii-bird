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

Pipe::Pipe()
{
  Pipe::y = rand() % 240 + 1 + (240 / 2);
  Pipe::x = 640;
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
  Pipe::y = rand() % 240 + 1 + (240 / 2);
  Pipe::x = 640;
}

// EOF
