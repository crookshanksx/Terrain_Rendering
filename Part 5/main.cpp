#include <GL/freeglut.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <cmath>
#include <cstdlib>
#include <cstdio>

// Grid size
const int gridSize = 50;
float heightMap[gridSize][gridSize];

// Camera position
float cameraX = 25.0f, cameraY = 10.0f, cameraZ = 50.0f;

// Texture ID
GLuint texture;

// Generate a wavy heightmap
void generateHeightMap() {
    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            float x = (float)i / gridSize;
            float y = (float)j / gridSize;
            heightMap[i][j] = (sinf(10 * x) + cosf(10 * y)) / 2.0f;
        }
    }
}

// Load a texture using stb_image
GLuint loadTexture(const char* filename) {
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
    
    if (!data) {
        printf("Failed to load texture: %s\n", filename);
        return 0;
    }

    printf("Texture loaded: %s (%d x %d, %d channels)\n", filename, width, height, nrChannels);

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    gluBuild2DMipmaps(GL_TEXTURE_2D, format, width, height, format, GL_UNSIGNED_BYTE, data);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return texID;
}

// Draw terrain using LOD with Quadtree-like detail reduction
void drawTerrain() {
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor3f(1.0f, 1.0f, 1.0f);

    for (int i = 0; i < gridSize - 1; i++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j < gridSize; j++) {
            float distance = sqrtf((i - gridSize / 2) * (i - gridSize / 2) + 
                                   (j - gridSize / 2) * (j - gridSize / 2));
            float lodFactor = (distance > 30.0f) ? 0.5f : 1.0f;

            glTexCoord2f((float)i / gridSize, (float)j / gridSize);
            glVertex3f(i, heightMap[i][j] * lodFactor, j);

            glTexCoord2f((float)(i + 1) / gridSize, (float)j / gridSize);
            glVertex3f(i + 1, heightMap[i + 1][j] * lodFactor, j);
        }
        glEnd();
    }
}

// Handle keyboard input
void keyPress(unsigned char key, int x, int y) {
    if (key == 'w') cameraZ -= 1.0f;
    if (key == 's') cameraZ += 1.0f;
    if (key == 'a') cameraX -= 1.0f;
    if (key == 'd') cameraX += 1.0f;
    glutPostRedisplay();
}

// Render scene
void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(cameraX, cameraY, cameraZ, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    
    drawTerrain();
    glutSwapBuffers();
}

// Initialize OpenGL
void initOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    GLfloat lightPos[] = { 0.0f, 50.0f, 50.0f, 1.0f };
    GLfloat lightColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 800.0 / 600.0, 1.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

// Window resize handler
void reshape(int width, int height) {
    if (height == 0) height = 1;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)width / (float)height, 1.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Terrain Rendering with Quadtree LOD");

    initOpenGL();
    generateHeightMap();

    texture = loadTexture("grass_texture.jpg"); // Ensure texture is available

    glutDisplayFunc(renderScene);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyPress);
    glutMainLoop();

    return 0;
}