# ASCII Knight

![C++](https://img.shields.io/badge/Language-C%2B%2B-00599C?style=flat-square&logo=cplusplus&logoColor=white)
![Platform](https://img.shields.io/badge/Platform-Windows-0078D6?style=flat-square&logo=windows&logoColor=white)
![Type](https://img.shields.io/badge/Type-Console%20Game-4D4D4D?style=flat-square)

A C++ console action-platformer where you control a hero (`@`) who runs, jumps, and attacks in four directions against escalating waves of enemies, culminating in a final boss fight.

Developed as **Course Project #10** for *Introduction to Programming (Practicum)* in Software Engineering at the Faculty of Mathematics and Informatics, Sofia University "St. Kliment Ohridski" (2025/2026).

---

## Demo

<!-- Replace the path below with your actual screenshot or gameplay GIF -->
![ASCII Knight Gameplay](assets/demo.gif)

---

## Gameplay

You spawn at the center of a color-coded, randomly generated arena with **5 HP** (displayed above the playfield). After a brief delay, enemies appear in waves, with each wave adding **2 to 4** more foes. The final wave features a single **Boss**.

* **Combat:** Successful attacks eliminate enemies; taking contact damage reduces your HP.
* **Win / Loss:** Reaching 0 HP triggers `GAME OVER`. Defeating the Boss clears the level.

## Controls

| Key | Action |
| :--- | :--- |
| `A` / `D` | Move left / right |
| `W` | Jump (press again mid-air to double jump) |
| `I` / `K` | Attack up / down |
| `J` / `L` | Attack left / right |

Attacks render briefly in the chosen direction and have a short cooldown between uses.

## Game Elements

### Arena

| Symbol | Element | Description |
| :---: | :--- | :--- |
| `@` | Player | Controlled by the user |
| `=` | Platform | Solid surface you can stand on (impassable) |
| `#` | Border | Impassable arena boundary |

### Enemies

Enemy movement tracks the player by comparing X and Y coordinates.

| Symbol | Type | Behavior |
| :---: | :--- | :--- |
| `E` | Basic Walker | Patrols platforms left and right |
| `J` | Jumper | Leaps when the player is nearby |
| `F` | Flier | Flies and periodically swoops down toward the player |
| `C` | Crawler | Clings to walls |
| `B` | Boss | Occupies a 3×3 area, takes multiple hits, and follows a unique behavior pattern |

### Player Attacks

```text
  Up      Left     Down     Right

 /-\       /                  \
           |       \_/        |
           \                  /
```

## Requirements & Running

Because the game relies on `conio.h` and `windows.h`, it runs on **Windows only** (using Visual Studio, MinGW, or any compatible C++ compiler). Ensure your console window fits at least **70×20** characters so the arena renders properly.

**Visual Studio**
1. Clone the repository.
2. Create a new C++ Console App project and add the source files.
3. Build and run the project.

**MinGW**
```bash
g++ main.cpp -o ascii_knight.exe
ascii_knight.exe
```

## Architecture

The codebase is organized by responsibility:

* **Initialization:** `InitializeArena`, `InitializePlayer`, `InitializeEnemies`
* **Input & Physics:** `CharacterControl`, `Check_Fall`, `Jump_Max_Height`
* **Enemies:** `AddEnemy`, `KillEnemy`, `UpdateEnemies`, `EnemyCollisionCheck`, plus type-specific handlers (`UpdateWalker`, `UpdateJumper`, `UpdateFlier`, `UpdateCrawler`, `UpdateBoss`)
* **Rendering:** `Graphics`, `ColorCharacters`, `ColorAttribute`, `ResetCursor`, `HideCursor`
* **Arena Generation:** `Arena_Template`, `RandomPlatforms`

Enemies are stored in dynamically allocated arrays (position, velocity, type, HP), while player state is held in a `std::vector`. All entities obey gravity and collide with platforms and walls inside a main loop that handles input, updates state, and redraws every **100 ms**.

### Libraries Used

Per project requirements, only the following libraries are used:
* `<iostream>` — console output
* `<vector>` — player state management
* `<ctime>` — timing, animation delays, and attack cooldowns
* `<conio.h>` — real-time keyboard input
* `<cstdlib>` — random number generation
* `<windows.h>` — text coloring and cursor control

## Author

Georgi Nikolaev Dimitrov, 2nd year at Faculty of Mathematics and Informatics, Sofia University
