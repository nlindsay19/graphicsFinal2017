#ifndef CUBE_H
#define CUBE_H

#include "scenegraph/Shape.h"

class Cube : public Shape
{
public:

    Cube();
    Cube(int i);
    ~Cube();

    /** Creates the vector that contains all the vertices for the cube to be used in the VBO */
    std::vector<float> getVertices() override;
    void initialize() override;

};

#endif // CUBE_H
