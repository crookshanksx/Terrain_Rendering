#include <GL/freeglut.h>
#include <cmath>
#include <cstdlib>

// Grid size
const int gridSize = 50;
float heightMap[gridSize][gridSize];

// Camera position
float cameraX = 0.0f, cameraY = 5.0f, cameraZ = 40.0f;

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

// Draw the terrain using the heightmap
void drawTerrain() {
    glColor3f(0.2f, 0.8f, 0.2f);  // Greenish color for terrain

    for (int i = 0; i < gridSize - 1; i++) {
        glBegin(GL_TRIANGLE_STRIP);
        for (int j = 0; j < gridSize; j++) {
            float distance = sqrtf((i - gridSize / 2) * (i - gridSize / 2) + 
                                   (j - gridSize / 2) * (j - gridSize / 2));
            float lodFactor = (distance > 30.0f) ? 0.5f : 1.0f;  // Lower detail for distant terrain

            glVertex3f(i, heightMap[i][j] * lodFactor, j);
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
    glutCreateWindow("Terrain Rendering with LOD");

    initOpenGL();
    generateHeightMap();

    glutDisplayFunc(renderScene);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyPress);
    glutMainLoop();

    return 0;
}