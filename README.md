# FigScroll (fs)

`fs` (FigScroll) is a terminal animation program that scrolls Figlet-generated ASCII art from right to left, inspired by the classic `sl` (steam locomotive) program by Toyoda Masashi. It dynamically generates ASCII art from user-specified text, font, and width, and supports infinite scrolling and adjustable speed.

## Features
- Generate ASCII art using `figlet` with custom text, font, and width.
- Scroll art across the terminal, even in narrow windows (like `sl`).
- Options:
  - `-t "text"`: Text to render (required). 
  - You'll need to escape special characters with `\` (e.g. `-t "test\!"`).
  - Be sure to include trailing space(s) in infinite scroll (e.g., `fs -t "text\! " -i`).
  - `-f font`: Figlet font (default: standard).
  - `-w width`: Figlet output width (default: 132).
  - `-i`: Infinite scroll, where new instances start before the previous one exits.
  - `-s microseconds`: Animation speed (default: 40000, ~25 FPS).
- Exit with 'q' or Ctrl+C.

## Prerequisites
- `figlet`: To generate ASCII art.
- `libncurses`: For terminal rendering (e.g., `libncurses5-dev` or `libncurses6-dev` on Debian/Ubuntu).
- Install on Debian/Ubuntu:
  ```bash
  sudo apt install figlet libncurses5-dev
  ```
- Install on macOS (via Homebrew):
  ```bash
  brew install figlet ncurses
  ```

## Installation

### Option 1: Build Locally
Build the `fs` binary in the current directory:
```bash
gcc -o fs fs.c -lncurses
```
Run it directly:
```bash
./fs -t "Hello World" -f slant
```

### Option 2: Install System-Wide
Clone the repository:
```bash
git clone https://github.com/[your-username]/figscroll.git
cd figscroll
```
Build and install to `/usr/local/bin` and `/usr/local/share/man/man1`:
```bash
make
sudo make install
```
Run from anywhere:
```bash
fs -t "be right back...    " -i
```
View the man page:
```bash
man fs
```
Uninstall:
```bash
sudo make uninstall
```

### Option 3: Install via Homebrew
Add the custom tap:
```bash
brew tap [your-username]/figscroll
```
Install `fs`:
```bash
brew install fs
```
Run and view the man page as above.

## Usage
```bash
fs -t "Hello World" -f slant -i
fs -t "be right back...    " -f roman -w 200 -s 20000
man fs  # View man page after installation
```

## Credits
- **Author**: [Your Name/Handle]
- **Inspired by**: `sl` by Toyoda Masashi (https://github.com/mtoyoda/sl)
- Built with `figlet` and `ncurses`.

## License
MIT License for original contributions, with portions derived from `sl` under its original terms (see LICENSE file).

## Acknowledgments
Thanks to Toyoda Masashi for the original `sl` program, which provided the core animation logic and inspiration for `fs`.
```

### What to Do Next
1. Save it to your project:
   ```bash
   cd ~/Downloads/figscroll
   nano README.md
   ```
   Paste the content inside the ```markdown

2. Build and test:
   ```bash
   make clean
   gcc -o fs fs.c -lncurses
   ./fs -t "Hello World" -f slant -i > test_new.txt
   ```
   Check `test_new.txt` with `less -R test_new.txt` to verify.

3. Install (optional):
   ```bash
   make
   sudo make install
   fs -t "Hello World" -f slant -i
