// src/audio.hpp
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

#include <memory>
#include "voice.hpp"
#include "sound.hpp"

class Audio
{
public:
  Audio();
  ~Audio();

  void PlayFlap();
  void PlayScore();
  void PlayHit();
  void PlayFall();
  void PlayTransition();

private:
  // Voices (Channels)
  std::unique_ptr<Voice> voice_flap;
  std::unique_ptr<Voice> voice_score;
  std::unique_ptr<Voice> voice_hit;
  std::unique_ptr<Voice> voice_fall;
  std::unique_ptr<Voice> voice_transition;

  // Sounds (Data)
  std::unique_ptr<Sound> sound_flap;
  std::unique_ptr<Sound> sound_score;
  std::unique_ptr<Sound> sound_hit;
  std::unique_ptr<Sound> sound_fall;
  std::unique_ptr<Sound> sound_transition;

  // Helper to parse WAV data from memory
  std::unique_ptr<Sound> LoadWav(const u8* data, u32 size);
};

// EOF
