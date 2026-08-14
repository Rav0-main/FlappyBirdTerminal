# 🐦 Flappy Bird Terminal

![C++20](https://img.shields.io/badge/C%2B%2B-20-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-064F8C?style=for-the-badge&logo=cmake&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![ncurses GNU](https://img.shields.io/badge/ncurses-GNU-4E9A06?style=for-the-badge&logo=gnu-bash&logoColor=white)

## 𓂃🪶 About

**Description**: A custom implementation of the classic game *Flappy Bird* in the *Linux terminal*.
The rendering, physics, and gameplay were written from scratch.

**Gameplay**:
<video src="https://github.com/user-attachments/assets/9a185900-9318-4ef8-a001-0e5cd611fa39" autoplay loop muted playsinline width="100%"></video>

## 📌 Features

*The key feature* of the implementation lies in *the flexibility of the game configuration*.
You can:
1) change the direction of gravity.
2) change the appearance of the pillow & bird.
3) change the position of the bird on the screen.\
... **etc**.

## 🛠️ Requirements (tools)

**Tools**:
* **CMake** >= **3.10**
* **clang++** *OR* **g++** >= **10.0** (which supports C++20)
* **ncursesw** >= **5.0**
* **Ubuntu / Debian Linux**.

## 🚀 Build & Installation

**To build** this application You need launch `build.sh`. **If everything went well**, You will see the file `FlappyBirdTerminal` **in the root directory**. This is a full‑fledged game. You can launch it in terminal.

**Installation & build**:
```sh
# OR SSH: git clone git@github.com:Rav0-main/FlappyBirdTerminal.git
git clone https://github.com/Rav0-main/FlappyBirdTerminal.git && cd FlappyBirdTerminal

# Do build-script executable
chmod +x ./build.sh
chmod +x ./install.sh

# Install, build and launch application
./install.sh && ./build.sh && ./FlappyBirdTerminal
```

## 🧩 Application structure

```
.
├── install.sh                     # install application requirements
├── build.sh                       # build application
├── .clang-format                  # C++ format of application
├── CMakeLists.txt
├── .gitignore
├── include
│   ├── best_score_manager.h       # header of class which handles best score of player
│   ├── bird.h                     # template class of player (bird)
│   ├── config.h                   # GAME SETTINGS: change it to customize
│   ├── fixed_cycle_buffer.h       # template class of cycle buffer
│   ├── icollision.h               # object interface that has collision 
│   ├── idrawable.h                # object interface that can draw at screen
│   ├── imovable.h                 # object interface that can move at screen
│   ├── irectangle2d.h             # object interface that has rectangle hitbox
│   ├── iscreen2d.h                # interface of screen that has axies: xOy
│   ├── pillow.h                   # template class of pillow
│   ├── pillow_random_manager.h    # class that generates pillow parameters
│   ├── randomizer.h               # template class that generates integers
│   ├── screen.h                   # header of class that implements terminal screen
│   ├── screen_rectangle.h         # contains template class which rectangle border of game screen
│   ├── text.h                     # template class for output text at screen
│   └── timer.h                    # header of class that handles time (secs)
├── README.md
└── src
    ├── best_score_manager.cpp
    ├── main.cpp
    ├── pillow_random_manager.cpp
    ├── screen.cpp
    └── timer.cpp
```

## ❤ Thank You for reading