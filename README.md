# Space Port - Computer Graphics Project

A GLUT/OpenGL 2D animated scene of a space port with a day/night cycle, moving vehicles, and a rocket launch. The scene is rendered at 1920x1080 using an orthographic projection.

## Features
- Day/night toggle with sun or moon and stars
- Animated clouds, airplane, windmill, and water flow
- Road traffic with cars and a school bus in two lanes
- River with moving boats
- Rocket launch and reset
- Adjustable animation speed

## Controls
- D: day mode
- N: night mode
- L: launch the rocket
- R: reset the rocket
- Up arrow: increase animation speed
- Down arrow: decrease animation speed
- Esc: quit

## Requirements
- C++ compiler (g++)
- OpenGL, GLU, and GLUT (freeglut)

## Build and Run (Linux)
```bash
sudo apt-get install freeglut3-dev
mkdir -p build
g++ main.cpp -o build/space-port -lGL -lGLU -lglut
./build/space-port
```

## Code::Blocks
- Open Computer-Graphics-Project.cbp in Code::Blocks.
- Build and run the project from the IDE.

## Project Structure
- main.cpp: drawing, animation, and input handling
- Computer-Graphics-Project.cbp: Code::Blocks project file

## Notes
- The window size is fixed at 1920x1080. If it does not fit your display, adjust the size in main.cpp.
