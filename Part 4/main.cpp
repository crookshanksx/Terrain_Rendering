

#include <GL/freeglut.h>
#include "quadtree.h"

// Camera position
float cameraX = 0.0f, cameraY = 50.0f, cameraZ = 50.0f;

Quadtree quadtree(0.0f, 0.0f, 50.0f, 5);  // Position (0, 0), size 50, 5 levels

void initOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);  // Use the default light

    // Set up light properties
    GLfloat lightPos[] = { 1.0f, 1.0f, 1.0f, 0.0f };  // Light position
    GLfloat lightColor[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Light color (white)
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);

    // Set material properties (terrain)
    GLfloat matSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat matShininess[] = { 50.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);

    // Set perspective
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 800.0 / 600.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(cameraX, cameraY, cameraZ, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    // Render terrain using quadtree LOD
    quadtree.render(cameraX, cameraY, cameraZ, 50.0f);  // Adjust viewDistance as needed

    glutSwapBuffers();
}

void update(int value) {
    // Simple camera movement (move forward)
    cameraZ -= 1.0f;

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);  // 16 ms for ~60 fps
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Terrain LOD with Quadtree");

    initOpenGL();

    glutDisplayFunc(renderScene);
    glutTimerFunc(25, update, 0);

    glutMainLoop();
    return 0;
}