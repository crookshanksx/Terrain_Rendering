#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <GL/freeglut.h>
#include <iostream>
#include <vector>

const int WIDTH = 256;
const int HEIGHT = 256;
float heightMap[WIDTH][HEIGHT];

float camX = 128.0f, camY = 50.0f, camZ = 300.0f;  // Initial camera position
float camSpeed = 5.0f;  // Camera movement speed

void loadHeightmap(const char* filename) {
    int imgWidth, imgHeight, channels;
    unsigned char* data = stbi_load(filename, &imgWidth, &imgHeight, &channels, 1);
    if (!data) {
        std::cerr << "Failed to load heightmap: " << filename << std::endl;
        exit(1);
    }

    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            int index = (y * imgWidth) + x;
            heightMap[x][y] = (data[index] / 255.0f) * 10.0f;
        }
    }
    stbi_image_free(data);
}

void drawTerrain() {
    glColor3f(0.3f, 0.8f, 0.3f);
    
    for (int y = 0; y < HEIGHT - 1; y++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int x = 0; x < WIDTH; x++) {
            glVertex3f(x, heightMap[x][y], y);
            glVertex3f(x, heightMap[x][y + 1], y + 1);
        }
        glEnd();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(camX, camY, camZ, 128, 0, 128, 0, 1, 0);

    drawTerrain();
    
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':  // Move forward
            camZ -= camSpeed;
            break;
        case 's':  // Move backward
            camZ += camSpeed;
            break;
        case 'a':  // Move left
            camX -= camSpeed;
            break;
        case 'd':  // Move right
            camX += camSpeed;
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Heightmap Terrain with FreeGLUT");

    glEnable(GL_DEPTH_TEST);

    loadHeightmap("heightmap.png");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);  // Register keyboard function
    glutMainLoop();
    
    return 0;
}