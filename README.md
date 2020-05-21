# OpenGL 3D Landscape

This project was made for assignment 2 in the course Graphics Programming - IMT2531.
![picture](resources/images/player.png)
![picture](resources/images/fog.png)
![picture](resources/images/day.png)
## How to setup

Make sure you have [cmake](https://cmake.org/) version 3.14 or later installed.
Navigate to the root folder of the project and execute the following commands

On linux:
```
mkdir build
cd build
cmake ..
make
./Exam
```

On windows:
```
mkdir build
cd build
cmake ..
cmake --build .
cd ..
cd out/build/x64-Debug
Exam.exe
```

## Controls

W - Move player forward

A - Move player left

S - Move player backwards

D - Move player right

F - Toggle flashlight

M - Switch between first-person, third-person and top down view

T - If on first-person press T to switch to a all around movable camera

LShift - Sprint

Esc - Pause the game

Arrow keys - Navigate the menu

Space - Make a selection in the menu

## Functionality
