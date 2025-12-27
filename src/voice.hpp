// src/voice.hpp
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

#include <gctypes.h>
#include "sound.hpp"

// Forward declarations
struct aesndpb_t;

class Voice
{
private:
  aesndpb_t *_Voice;

public:
  Voice();
  Voice(Voice const&) = delete;
  virtual ~Voice();
  Voice& operator=(Voice const&) = delete;

  void SetVolume(u16 Volume);
  void SetVolume(u16 LeftVolume, u16 RightVolume);
  void Play(const Sound& sound, u32 delay = 0, bool looped = false);
  void Stop();
  void Mute(bool mute);
};

// EOF
