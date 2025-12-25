// src/collision.hpp
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

struct Hitbox
{
  float x;      // Left edge
  float y;      // Top edge
  float width;
  float height;

  // Constructor
  Hitbox(float x = 0, float y = 0, float width = 0, float height = 0)
    : x(x), y(y), width(width), height(height)
  {
  }

  // Getters for edges (makes collision logic clearer)
  float left() const
  {
    return x;
  }
  float right() const
  {
    return x + width;
  }
  float top() const
  {
    return y;
  }
  float bottom() const
  {
    return y + height;
  }

  // Check if this hitbox intersects another
  bool intersects(const Hitbox& other) const
  {
    return !(right() < other.left() ||
             left() > other.right() ||
             bottom() < other.top() ||
             top() > other.bottom());
  }

  // Check if this hitbox is outside screen bounds
  bool is_out_of_bounds(float screen_width, float screen_height) const
  {
    return (top() < 0 ||
            bottom() > screen_height ||
            left() < 0 ||
            right() > screen_width);
  }
};

// EOF
