#include <GL/freeglut.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"  // Include stb_image header
#include <cmath>
#include <cstdlib>

// Grid size
const int gridSize = 50;
float heightMap[gridSize][gridSize];

// Camera position
float cameraX = 0.0f, cameraY = 5.0f, cameraZ = 40.0f;

// Texture ID
GLuint texture;

// Generate a simple wavy heightmap
void generateHeightMap() {
    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            float x = (float)i / gridSize;
            float y = (float)j / gridSize;
            heightMap[i][j] = (sinf(10 * x) + cosf(10 * y)) / 2.0f;  // Wavy pattern
        }
    }
}

// Load a texture using stb_image.h
GLuint loadTexture(const char* filename) {
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (data) {
        GLuint texID;
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data); // Generate mipmaps for the texture

        // Set texture wrapping and filtering options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data); // Free the image data after uploading it to OpenGL
        return texID;
    } else {
        printf("Failed to load texture: %s\n", filename);
        return 0;
    }
}

// Draw the terrain using the heightmap
void drawTerrain() {
    glBindTexture(GL_TEXTURE_2D, texture);  // Bind the texture
    glColor3f(1.0f, 1.0f, 1.0f);  // Set color to white for texture
    
    for (int i = 0; i < gridSize - 1; i++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j < gridSize; j++) {
            float distance = sqrtf((i - gridSize / 2) * (i - gridSize / 2) + 
                                   (j - gridSize / 2) * (j - gridSize / 2));
            float lodFactor = (distance > 30.0f) ? 0.5f : 1.0f;  // Lower detail for distant terrain

            // Set texture coordinates
            glTexCoord2f((float)i / gridSize, (float)j / gridSize);
            glVertex3f(i, heightMap[i][j] * lodFactor, j);

            glTexCoord2f((float)(i + 1) / gridSize, (float)j / gridSize);
            glVertex3f(i + 1, heightMap[i + 1][j] * lodFactor, j);
        }
        glEnd();
    }
}

// Handle keyboard input for camera movement
void keyPress(unsigned char key, int x, int y) {
    if (key == 'w') cameraZ -= 1.0f;  // Move forward
    if (key == 's') cameraZ += 1.0f;  // Move backward
    if (key == 'a') cameraX -= 1.0f;  // Move left
    if (key == 'd') cameraX += 1.0f;  // Move right
    glutPostRedisplay();
}

// Render the scene with camera
void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(cameraX, cameraY, cameraZ,  // Camera position
              0.0f, 0.0f, 0.0f,           // Look at point
              0.0f, 1.0f, 0.0f);          // Up direction

    drawTerrain();
    glutSwapBuffers();
}

// Initialize OpenGL settings
void initOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);  // Enable textures
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 800.0 / 600.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// Handle window resize
void reshape(int width, int height) {
    if (height == 0) height = 1;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)width / (float)height, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Terrain Rendering with Texture and LOD");

    initOpenGL();
    generateHeightMap();

    // Load the texture
    texture = loadTexture("grass_texture.jpg");  // Replace with your texture filename

    glutDisplayFunc(renderScene);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyPress);
    glutMainLoop();

    return 0;
}