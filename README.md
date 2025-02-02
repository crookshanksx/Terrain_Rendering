# Terrain Rendering with Level of Detail (LoD) and Texture Mapping

This project demonstrates terrain rendering using OpenGL with Level of Detail (LoD) techniques and texture mapping. The terrain is generated based on a simple heightmap and is textured using an image. The project allows for real-time camera movement and LoD adjustments based on the camera's position.

## Table of Contents

1. [Project Overview](#project-overview)
2. [Installation Instructions](#installation-instructions)
3. [Usage](#usage)
4. [Project Structure](#project-structure)
5. [Key Components](#key-components)
6. [License](#license)

## Project Overview

This project renders a dynamic terrain using OpenGL, employing techniques such as Level of Detail (LoD) for performance optimization and texture mapping for realistic terrain visualization. The terrain is generated using a heightmap and features real-time camera movement.

### Features:
- **Terrain rendering** using a heightmap with LoD.
- **Texture mapping** using external images (requires stb_image.h).
- **Real-time camera movement** with basic keyboard controls.
- **LoD adjustments** based on the distance from the camera to the terrain.

## Installation Instructions

### Prerequisites:
- OpenGL (with GLUT/FreeGLUT)
- stb_image.h for texture loading
- C++ compiler

### Steps to Run the Project:
1. Clone the repository:
    ```bash
    git clone https://github.com/yourusername/terrain-rendering.git
    cd terrain-rendering
    ```

2. Ensure you have the dependencies installed:
    - Install FreeGLUT (or GLUT)
    - Download the `stb_image.h` header file and place it in the project directory.

3. Compile and run the project:
    ```bash
    g++ -o terrain-rendering main.cpp -lGL -lGLU -lglut -lX11 -lm
    ./terrain-rendering
    ```

    - Replace `main.cpp` with the actual file name if different.

4. Add a texture file (e.g., `grass_texture.jpg`) to the project directory.

## Usage

- **Camera Movement**: Use the **WASD** keys to move the camera:
  - `W` - Move forward
  - `S` - Move backward
  - `A` - Move left
  - `D` - Move right

- **LoD Effect**: The terrain's detail adjusts based on the camera's distance to the grid, providing performance optimization for distant terrain.

## Project Structure

```bash
terrain-rendering/
├── main.cpp          # Main source code file
├── stb_image.h       # Header for image loading (stb_image)
├── grass_texture.jpg # Example texture file (replace with your own)
└── README.md         # This file
