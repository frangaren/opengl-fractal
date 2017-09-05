# OpenGL Fractal

## Description
A implementation of Mandelbrot's and Julia's set in GLSL over C + OpenGL.

## How to build

### Linux
1. Install GLFW3 and GLEW:

  **Arch Linux**

  If you are using x11:
  ```
  sudo pacman -Syu glfw-x11 glew
  ```
  If you are using Wayland:
  ```
  sudo pacman -Syu glfw-wayland glew
  ```

2. Open a terminal and go to the directory where this readme is in.
3. Run `make` on the terminal.
4. The program is now compiled in the `bin` directory with all the shader
   files (`*.glsl`) needed to run the program.

## Command options
- `--fp64` and `-64`: use double precision arithmetic if possible.
- `--fullscreen` and `-f`: run on fullscreen.
- `--width` and `-w`: set width.
- `--height` and `-h`: set height.
- `--julia` and `-j`: show Julias's instead of Mandelbrot's one.
- `--help` and `-h`: print help message.

## Controls
- **w** and **up arrow**: move fractal up.
- **s** and **down arrow**: move fractal down.
- **a** and **left arrow**: move fractal left.
- **d** and **right arrow**: move fractal right.
- **r** and **left shift**: zoom out.
- **f** and **left control**: zoom in.
- **i**, **j**, **k** and **l**: modify Julia's fractal constant (only in
  Julia's fractal).
- **q** and **escape**: quit.
