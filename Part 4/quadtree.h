

#ifndef QUADTREE_H
#define QUADTREE_H

#include "quadnode.h"
#include <vector>

class Quadtree {
private:
    float x, y, size;
    int maxLevel;
    std::vector<QuadNode*> nodes;

public:
    Quadtree(float x, float y, float size, int maxLevel)
        : x(x), y(y), size(size), maxLevel(maxLevel) {
        // Generate the quadtree nodes
        generateNodes(x, y, size, 0);
    }

    // Recursive function to generate quadtree nodes
    void generateNodes(float x, float y, float size, int level) {
        if (level > maxLevel) return;

        // Add a new node to the list
        nodes.push_back(new QuadNode(x, y, size, level));

        // Subdivide the node into four smaller nodes
        generateNodes(x + size / 2, y, size / 2, level + 1);
        generateNodes(x, y + size / 2, size / 2, level + 1);
        generateNodes(x + size / 2, y + size / 2, size / 2, level + 1);
        generateNodes(x - size / 2, y + size / 2, size / 2, level + 1);
    }

    // Render all nodes in the quadtree
    void render(float cameraX, float cameraY, float cameraZ, float viewDistance) {
        for (QuadNode* node : nodes) {
            if (node->isInView(cameraX, cameraY, cameraZ, viewDistance)) {
                node->render();  // Render only nodes that are in view
            }
        }
    }
};

#endif