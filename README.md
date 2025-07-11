# Rand(maze) Adventure Game

A top-down roguelike action game built with C++ and SFML.
Explore procedurally generated dungeons using Binary Space Partitioning (BSP), fight enemies, collect 3 keys, and defeat the boss to unlock the final treasure chest.

![Game Preview](https://media4.giphy.com/media/v1.Y2lkPTc5MGI3NjExYTB0amY1eGxqbm81empteHZ1bHVkOHJ3MnpxdW9zNTlvbW9udGRpdSZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/Wnw2LivOiJ8j4DMEhp/giphy.gif)

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
