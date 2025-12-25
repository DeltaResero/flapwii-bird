// src/pipe.hpp
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

#include <stdlib.h>

class Pipe
{
private:
  float speed;

public:
  float x, y;

  Pipe();
  ~Pipe();
  void move();
  void reset();
};

// EOF
