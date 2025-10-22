# MathBoi
is a Raylib game that I wrote as a schoolproject for my C class.

## Controls
- You can type numbers normally
- Decimal pointer is a `.`
- Correct your mistakes with `space`

## Compilation
> [!WARNING] 
> All commands shall be run from the root directory of the project !

This project uses [Raylib](https://www.raylib.com/) for most of the functionality and fully for rendering (awesome library). Compiling this on Windows is not as straight forward as on linux, but in future, I'll provide the instructions necesary to compile on Windows.

### 1. Install Dependencies
#### Arch-based distributions (e.g., Arch Linux, Manjaro)

```bash
sudo pacman -Syu gcc make pkg-config raylib git
```

#### Debian-based distributions (e.g., Arch Linux, Manjaro)

```bash
sudo apt update
sudo apt install build-essential pkg-config libraylib-dev git
```

### 2. Clone the repository
```bash
git clone https://github.com/Lopastudio/MathBoi
```

### 3.1 Compile using make
```bash
make        # Compile the project to the ./bin directory
make run    # Compile and also run the project
make clean  # Remove the binaries
```

### 3.2 Compile using GCC without makefile
I have no idea why would anyone need this, but here it is:
```bash
mkdir -p bin
gcc -Wall -std=c99 main.c -o bin/mathboi $(pkg-config --cflags --libs raylib) -lm
```


## TODO:
- ~~Write the core~~
- ~~Add the actual core mechanic of the game~~
- ~~Add bottom field for showing what you typed~~
- ~~Add removing of wrong user input~~
- ~~Add background~~
- Scale background properly
- Add the gun that shots the problems down (player sprite)
- Add explosions when hit
- Add Soundeffects
- Add main menu
- Add high score
- Add difficulties
- Write an chiptune OST
- Add instructions for compilation under Michaelsoft Binbows :D