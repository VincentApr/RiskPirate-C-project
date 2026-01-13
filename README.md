# RiskPirate - C++ Project
A strategy game inspired by Risk with a pirate theme, developed in C++ using the Imagine++ library and Qt5.
Project created by Antoine Salomon, Axel Delmas, and myself, under the supervision of Pascal Monasse.

### File Description
Main Files

exo2.cpp: Game entry point. Contains the main loop, turn management, user interactions (keyboard/mouse), and initialization of the map and players.

iles.cpp / iles.h: Game map management (islands, trees, flags). Includes island drawing with water gradient, procedural generation of coconut trees, player flag placement, and loading of graphical resources.

partie.cpp / partie.h: Combat system between players. Implements the battle mechanism (coin flip), coin toss animation, soldier display, and combat resolution.
CMakeLists.txt: CMake configuration to compile the project with Imagine++, Qt5, and OpenGL dependencies.

### Controls

- **Mouse click**: Select an island
- **`a`**: Move soldiers between your islands
  - Click on the departure island
  - Click on the destination island
  - Type a digit (1-9) for the number of soldiers
- **`z`**: Attack an enemy island
  - Click on your attacking island
  - Click on the target island
- **`e`**: Place new soldiers (start of turn)
- **`r`**: Build a defensive tower on an island
- **Arrow keys**: Move the map view
- **`Enter`**: End your turn

### Rules

1. At the start, each player selects 5 islands in turn
2. Each turn, you receive soldiers based on the number of islands owned
3. Battles are resolved through successive coin flips
4. The goal is to conquer all enemy islands

---

## Dependencies

- **Imagine++**: Graphics library
- **Qt5** (Widgets, Core, Gui, OpenGL)


## Required Assets

The game requires the following graphical resources in the appropriate folders:
```
./cocotiers/
  ├── cocotier1.png to cocotier6.png
  ├── grayflag.png
  ├── blueflag.png
  └── redflag.png

../assets/
  ├── battlefield.jpg
  ├── soldier_red.png
  └── soldier_blue.png

📝 Technical Notes

The map generates 30 islands in total (10 visible + 20 off-screen for exploration)
Bridges between islands are procedurally generated with connectivity verification
The combat system uses references to directly modify troop counts
Coconut trees and flags display uses PNG images with alpha channel
