// src/audio.cpp
// SPDX-License-Identifier: GPL-3.0-or-later
//
// Flapwii Bird
// Copyright (C) 2025 DeltaResero
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

#include "audio.hpp"
#include <aesndlib.h>
#include <cstring>
#include <vector>

// Generated symbols from Makefile (bin2o)
extern "C" {
    extern const u8 sfx_flap_wav[];
    extern const u8 sfx_flap_wav_end[];

    extern const u8 sfx_score_wav[];
    extern const u8 sfx_score_wav_end[];

    extern const u8 sfx_hit_wav[];
    extern const u8 sfx_hit_wav_end[];

    extern const u8 sfx_die_wav[];
    extern const u8 sfx_die_wav_end[];

    extern const u8 sfx_transition_wav[];
    extern const u8 sfx_transition_wav_end[];
}

Audio::Audio()
{
  AESND_Init();
  AESND_Pause(false);

  // Initialize Voices
  voice_flap = std::make_unique<Voice>();
  voice_score = std::make_unique<Voice>();
  voice_hit = std::make_unique<Voice>();
  voice_die = std::make_unique<Voice>();
  voice_transition = std::make_unique<Voice>();

  // Load Sounds
  sound_flap = LoadWav(sfx_flap_wav, sfx_flap_wav_end - sfx_flap_wav);
  sound_score = LoadWav(sfx_score_wav, sfx_score_wav_end - sfx_score_wav);
  sound_hit = LoadWav(sfx_hit_wav, sfx_hit_wav_end - sfx_hit_wav);
  sound_die = LoadWav(sfx_die_wav, sfx_die_wav_end - sfx_die_wav);
  sound_transition = LoadWav(sfx_transition_wav, sfx_transition_wav_end - sfx_transition_wav);
}

Audio::~Audio()
{
  voice_flap.reset();
  voice_score.reset();
  voice_hit.reset();
  voice_die.reset();
  voice_transition.reset();

  AESND_Pause(true);
}

void Audio::PlayFlap()
{
  if (sound_flap)
  {
    voice_flap->SetVolume(200);
    voice_flap->Play(*sound_flap);
  }
}

void Audio::PlayScore()
{
  if (sound_score)
  {
    voice_score->SetVolume(255);
    voice_score->Play(*sound_score);
  }
}

void Audio::PlayHit()
{
  if (sound_hit)
  {
    voice_hit->SetVolume(255);
    voice_hit->Play(*sound_hit);
  }
}

void Audio::PlayDie()
{
  if (sound_die)
  {
    voice_die->SetVolume(255);
    voice_die->Play(*sound_die);
  }
}

void Audio::PlayTransition()
{
  if (sound_transition)
  {
    voice_transition->SetVolume(255);
    voice_transition->Play(*sound_transition);
  }
}

std::unique_ptr<Sound> Audio::LoadWav(const u8* data, u32 size)
{
  if (size < 44) return nullptr;

  auto read16 = [&](u32 offset) -> u16 {
    return data[offset] | (data[offset + 1] << 8);
  };
  auto read32 = [&](u32 offset) -> u32 {
    return data[offset] | (data[offset + 1] << 8) |
           (data[offset + 2] << 16) | (data[offset + 3] << 24);
  };

  if (memcmp(data, "RIFF", 4) != 0) return nullptr;
  if (memcmp(data + 8, "WAVE", 4) != 0) return nullptr;

  u32 ptr = 12;
  u32 fmt_size = 0;
  u16 channels = 0;
  u32 sample_rate = 0;
  u16 bits_per_sample = 0;

  while (ptr < size - 8)
  {
    if (memcmp(data + ptr, "fmt ", 4) == 0)
    {
      fmt_size = read32(ptr + 4);
      channels = read16(ptr + 10);
      sample_rate = read32(ptr + 12);
      bits_per_sample = read16(ptr + 22);
      break;
    }
    ptr++;
  }

  if (sample_rate == 0) return nullptr;

  ptr = 12 + 8 + fmt_size;
  u32 data_offset = 0;
  u32 data_len = 0;

  ptr = 12;
  while (ptr < size - 8)
  {
    if (memcmp(data + ptr, "data", 4) == 0)
    {
      data_len = read32(ptr + 4);
      data_offset = ptr + 8;
      break;
    }
    ptr++;
  }

  if (data_offset == 0 || data_offset + data_len > size) return nullptr;

  u32 format;
  std::vector<u8> audio_buffer;

  if (bits_per_sample == 8)
  {
    format = (channels == 2) ? VOICE_STEREO8 : VOICE_MONO8;
    audio_buffer.assign(data + data_offset, data + data_offset + data_len);
  }
  else if (bits_per_sample == 16)
  {
    format = (channels == 2) ? VOICE_STEREO16 : VOICE_MONO16;

    audio_buffer.resize(data_len);
    for (u32 i = 0; i < data_len; i += 2)
    {
      u8 low = data[data_offset + i];
      u8 high = data[data_offset + i + 1];
      audio_buffer[i] = high;
      audio_buffer[i + 1] = low;
    }
  }
  else
  {
    return nullptr;
  }

  return std::make_unique<Sound>(
    format,
    std::move(audio_buffer),
    static_cast<f32>(sample_rate)
  );
}

// EOF
