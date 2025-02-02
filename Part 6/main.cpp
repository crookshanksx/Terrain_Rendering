#include <GL/freeglut.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <cmath>
#include <cstdlib>
#include <vector>

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
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
        return texID;
    } else {
        printf("Failed to load texture: %s\n", filename);
        return 0;
    }
}

// Implement Level of Detail (LOD) for terrain
void drawTerrain() {
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor3f(1.0f, 1.0f, 1.0f);
    
    for (int i = 0; i < gridSize - 1; i += 2) {  // Lower resolution rendering with LOD
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j < gridSize; j += 2) {
            float distance = sqrtf((i - gridSize / 2) * (i - gridSize / 2) + 
                                   (j - gridSize / 2) * (j - gridSize / 2));
            float lodFactor = (distance > 30.0f) ? 0.25f : 1.0f;  // Lower detail for distant terrain

            glTexCoord2f((float)i / gridSize, (float)j / gridSize);
            glVertex3f(i, heightMap[i][j] * lodFactor, j);

            glTexCoord2f((float)(i + 2) / gridSize, (float)j / gridSize);
            glVertex3f(i + 2, heightMap[i + 2][j] * lodFactor, j);
        }
        glEnd();
    }
}

void keyPress(unsigned char key, int x, int y) {
    if (key == 'w') cameraZ -= 1.0f;
    if (key == 's') cameraZ += 1.0f;
    if (key == 'a') cameraX -= 1.0f;
    if (key == 'd') cameraX += 1.0f;
    glutPostRedisplay();
}

void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    drawTerrain();
    glutSwapBuffers();
}

void initOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 800.0 / 600.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void reshape(int width, int height) {
    if (height == 0) height = 1;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)width / (float)height, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Terrain Rendering with Texture and LOD");

    initOpenGL();
    generateHeightMap();
    texture = loadTexture("grass_texture.jpg");

    glutDisplayFunc(renderScene);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyPress);
    glutMainLoop();

    return 0;
}