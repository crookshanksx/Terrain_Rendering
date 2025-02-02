

#ifndef QUADNODE_H
#define QUADNODE_H

#include <GL/freeglut.h>
#include <cmath>

class QuadNode {
public:
    float x, y;  // Position of the node (bottom-left corner of the block)
    float size;  // Size of the node (half the size of the parent)
    int level;    // Level of detail (higher means more detail)

    QuadNode(float x, float y, float size, int level)
        : x(x), y(y), size(size), level(level) {}

    // Function to check if the node is in range of the camera
    bool isInView(float cameraX, float cameraY, float cameraZ, float viewDistance) {
        float dx = cameraX - x;
        float dz = cameraZ - y;
        float distance = sqrt(dx * dx + dz * dz);
        return distance < viewDistance;
    }

    // Render the node, choosing the LOD based on the level
    void render() {
        glPushMatrix();
        glTranslatef(x, 0.0f, y);  // Position the node in the scene

        glBegin(GL_QUADS);
        // Here we could render the terrain based on the LOD
        glVertex3f(0, 0, 0);
        glVertex3f(size, 0, 0);
        glVertex3f(size, 0, size);
        glVertex3f(0, 0, size);
        glEnd();

        glPopMatrix();
    }
};

#endif