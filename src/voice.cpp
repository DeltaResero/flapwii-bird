// src/voice.cpp
// SPDX-License-Identifier: GPL-3.0-or-later
//
// Flapwii Bird
// Copyright (C) 2025 DeltaResero
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

#include "voice.hpp"
#include <aesndlib.h>

Voice::Voice()
{
  _Voice = AESND_AllocateVoice(nullptr);
}

Voice::~Voice()
{
  AESND_FreeVoice(_Voice);
}

void Voice::SetVolume(u16 Volume)
{
  SetVolume(Volume, Volume);
}

void Voice::SetVolume(u16 LeftVolume, u16 RightVolume)
{
  AESND_SetVoiceVolume(_Voice, LeftVolume, RightVolume);
}

void Voice::Play(const Sound& sound, u32 delay, bool looped)
{
  AESND_PlayVoice(_Voice, sound.GetFormat(), sound.GetBufferPtr(),
                  sound.GetSize(), sound.GetFrequency(), delay, looped);
}

void Voice::Stop()
{
  AESND_SetVoiceStop(_Voice, true);
}

void Voice::Mute(bool mute)
{
  AESND_SetVoiceMute(_Voice, mute);
}

// EOF
