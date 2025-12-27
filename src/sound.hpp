// src/sound.hpp
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
#include <vector>

// Owns the audio buffer to allow for endianness conversion
class Sound
{
private:
  const u32 _format;
  std::vector<u8> _buffer;
  const f32 _freq;

public:
  Sound(u32 format, std::vector<u8> buffer, f32 frequency)
    : _format(format)
    , _buffer(std::move(buffer))
    , _freq(frequency)
  {
  }

  Sound(Sound const&) = delete;
  Sound& operator=(Sound const&) = delete;

  [[nodiscard]] const u8* GetBufferPtr() const
  {
    return _buffer.data();
  }

  [[nodiscard]] u32 GetSize() const
  {
    return _buffer.size();
  }

  [[nodiscard]] u32 GetFormat() const
  {
    return _format;
  }

  [[nodiscard]] f32 GetFrequency() const
  {
    return _freq;
  }
};

// EOF
