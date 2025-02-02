#include <GL/freeglut.h>
#include <iostream>

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Move the camera back to see the grid
    glTranslatef(-0.5f, -0.5f, -2.0f);

    glColor3f(0.0f, 1.0f, 0.0f);  // Green color for terrain

    // Draw a simple terrain grid
    for (float i = -1.0; i <= 1.0; i += 0.1) {
        glBegin(GL_LINES);
        glVertex3f(i, -1.0, 0.0);
        glVertex3f(i, 1.0, 0.0);
        glVertex3f(-1.0, i, 0.0);
        glVertex3f(1.0, i, 0.0);
        glEnd();
    }

    glutSwapBuffers();
}

void reshape(int width, int height) {
    if (height == 0) height = 1;
    float aspect = (float)width / (float)height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void initOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Terrain LOD - Basic Grid");

    initOpenGL();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}


