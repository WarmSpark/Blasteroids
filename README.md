# Blasteroids in C

A classic Blasteroids arcade game built in C using raylib.

## Features
- Ship movement with realistic thrust and drift physics
- Asteroid splitting (large → medium → small)
- Bullet firing with object pooling
- Circle collision detection
- Wave respawning system
- Lives and score tracking

## Controls
| Key | Action |
|-----|--------|
| W | Thrust |
| A | Rotate Left |
| D | Rotate Right |
| Space | Shoot |

## Build & Run
Make sure you have raylib installed:
```bash
sudo pacman -S raylib
```

Then build with CMake:
```bash
mkdir build && cd build
cmake ..
make
./test
```

## Project Structure
```
├── main.c          # game loop, wave management
├── player.c/h      # ship movement and rendering
├── bullets.c/h     # bullet firing and pooling
├── aestroids.c/h   # asteroid spawning and splitting
└── collision.c/h   # collision detection
```
