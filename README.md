# Flapwii Bird

Flapwii Bird is a recreation for the Wii of the viral hit [*Flappy Bird*](
https://en.wikipedia.org/wiki/Flappy_Bird). It's designed for use with the
[Homebrew Channel](https://wiibrew.org/wiki/Homebrew_Channel). While not an
exact clone, the game attempts to capture the fun and addictively frustrating
nature of the classic gameplay. It offers a deceptively simple experience that
functionally mirrors the original's 2011 mechanics.

## About This Version

This is a fork of the original [Flapwii Bird](https://github.com/TheBlueOompaLoompa/flapwii-bird)
by [TheBlueOompaLoompa](https://github.com/TheBlueOompaLoompa). The original was
written over the course of 24 hours as a way to rediscover the homebrew scene
and stands as a testament to the creator's goal to "give it a good try and
actually succeed" at Wii development. The goal of this continuation fork is to
modernize the codebase and continue development with new features and fully
open source assets.

<br>

[![Flapwii Bird Gameplay Screenshot][gameplay-screenshot]][gameplay-screenshot]

[![Latest Release][release-img]][release-url]
[![View All Releases][downloads-img]][downloads-url]
[![License](https://img.shields.io/badge/license-GPLv3+-blue.svg)](LICENSE)
[![Codacy Badge][codacy-img]][codacy-url]

<!-- Link Definitions -->
[gameplay-screenshot]: extras/Screenshot_Gameplay-12-24-2025.png
[release-img]: https://img.shields.io/github/v/release/DeltaResero/flapwii-bird?label=Latest%20Release
[release-url]: https://github.com/DeltaResero/flapwii-bird/releases/latest
[downloads-img]: https://img.shields.io/badge/Downloads-View_All_Releases-blue
[downloads-url]: https://github.com/DeltaResero/flapwii-bird/releases
[codacy-img]: https://app.codacy.com/project/badge/Grade/7be32ebc2796441da2bdbc3f889100af
[codacy-url]: https://app.codacy.com/gh/DeltaResero/flapwii-bird/dashboard

<br><br>

## How to Build

### Prerequisites

To build Flapwii Bird, you'll need the **devkitPro devkitPPC** toolchain
installed. To set up the devkitPro devkitPPC PowerPC build system, follow the
instructions on the official devkitPro wiki:

- [Getting Started with devkitPro](https://devkitpro.org/wiki/Getting_Started)
- [devkitPro Pacman](https://devkitpro.org/wiki/devkitPro_pacman)

After setting up devkitPPC including environment variables, use (dkp-)pacman
to install the following dependencies:

```bash
libogc
libfat-ogc
ppc-freetype
ppc-bzip2
ppc-libpng
ppc-libjpeg-turbo
ppc-zlib
ppc-libvorbisidec
ppc-libogg
```

Note: The **GRRLIB** dependency is downloaded automatically by the Makefile; you
do not need to install it manually.

### Build Instructions

1. Clone the repository to your local machine.
2. Open a terminal or command prompt in the project's root directory.
3. Run the command `make`.

The build process should create a `boot.dol` file inside the `apps/flapwii`
directory if the devkitPPC toolchain and dependencies were all set up and
installed properly.

<br>

## How to Install

1. Ensure you have the [Homebrew Channel](https://wiibrew.org/wiki/Homebrew_Channel)
   installed on your Wii.
2. Copy the `apps` folder from the project root to the root of your SD card or
   USB drive.
3. Copy the generated `boot.dol` file to the `apps/flapwii` folder on the SD
   card.
4. Ensure the `apps/flapwii` folder contains `boot.dol`, `icon.png`, and
   `meta.xml`.
5. Insert the SD card or USB drive into your Wii and launch Flapwii Bird from
   the Homebrew Channel.

### Cleaning the Build

If you need to clean up build artifacts (such as for rebuilding), run
`make clean`. To remove downloaded third-party libraries (GRRLIB) as well,
run `make distclean`.

<br>

## Disclaimer and Licensing

This is an unofficial continuation of Flapwii Bird that runs on the Wii via the
Homebrew Channel. It is not affiliated with, endorsed by, nor sponsored by the
creators of the Wii console nor the Homebrew Channel. All trademarks and
copyrights are the property of their respective owners.

The source code for this project is distributed under the terms of the GNU
General Public License version 3 (or later). You can redistribute it and/or
modify it under the terms of the GNU General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your option)
any later version.

This project is distributed in the hope that it will be useful, but **WITHOUT
ANY WARRANTY**; without even the implied warranty of **MERCHANTABILITY** or
**FITNESS FOR A PARTICULAR PURPOSE**. See the [GNU General Public License](
https://www.gnu.org/licenses/gpl-3.0.en.html) for more details.

The custom sound assets created for this project are dedicated to the public
domain under the [Creative Commons CC0 1.0 Universal license](
https://creativecommons.org/publicdomain/zero/1.0/). Graphical assets and fonts
are inherited from the original Flapwii, though their specific licensing terms
and origins remain unknown.
