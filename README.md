# Rand(maze) Adventure Game

A top-down roguelike action game built with C++ and SFML.
Explore procedurally generated dungeons using Binary Space Partitioning (BSP), fight enemies, collect 3 keys, and defeat the boss to unlock the final treasure chest.

![Game Preview](assets/game-preview.gif)

---

## Play The Game

Windows: 

Download & Extract the game: [download link](https://www.dropbox.com/scl/fi/4v6q7cxe58hkpcgeat7r0/Rand-maze-Adventure.zip?rlkey=1vgti54svkf3ihwb6vmmcumvm&st=e0qmikt3&dl=0)

---

## Dependencies
  - SFML v3.0
  - Background Music: [An-Instant-Burst-Action-02](https://onemansymphony.bandcamp.com/track/an-instant-burst-action-02) composed by One Man Symphony
  - Assets: [Kenny Assets](https://kenney.nl/assets)

---

## Build Instruction

### Install SFML v3.0

Windows: Download from official website: https://www.sfml-dev.org/download/sfml/3.0.0/

Linux: There is still no easy way to install sfml v3.0 in linux. You can get is from the official SFML repo: https://github.com/SFML/SFML

### Build Command

Windows:

`<COMPILER_PATH\g++.exe> -std=c++17 -fdiagnostics-color=always -g main.cpp mapgen.cpp player.cpp bsp_algorithm.cpp enemy.cpp audio.cpp timer.cpp pause.cpp options.cpp view_manager.cpp credits.cpp menu.cpp leaderboard.cpp controls.cpp -I<SFML_INCLUDE_PATH> -DSFML_STATIC -o "main.exe" -L<SFML_LIB_PATH> -lsfml-graphics-s -lsfml-window-s -lsfml-audio-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -lflac -lvorbisenc -lvorbisfile -lvorbis -logg -static-libgcc -static-libstdc++`



---

## About

This is a project for CSE-1202:Structured Programming Course
