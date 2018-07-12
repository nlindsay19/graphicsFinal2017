#include "Cube.h"

#include "ui/Settings.h"
#include <vector>
#include <stdio.h>

#include "lib/ResourceLoader.h"
#include "Shape.h"


Cube::Cube() :
    Shape()
{

    std::vector<float> vec = getVertices();
    setVertexData(vec.data(), vec.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES, vec.size()/3);
    setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    setAttribute(ShaderAttrib::NORMAL, 3, 12, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    buildVAO();
    draw();

}

Cube::Cube(int i) :
    Shape()
{

}

Cube::~Cube()
{

}

/*
 * This function calculates and inserts all the vertices for the shape. It uses the VertexHandler
 * object to insert each triangle, passing the coordinates of each vertex into its insertTriangle
 * function.
 */
std::vector<float> Cube::getVertices() {

    // Getting parameter 1 and setting up vector for vertices
    int p1 = settings.shapeParameter1;
    std::vector<float> vec = {};

    // Increment to use to iterate along sides of cube
    float incr = 1.0 / p1;

    // Creating normals for each face
    std::vector<float> xy_pos_normal = {0, 0, 1};
    std::vector<float> xy_neg_normal = {0, 0, -1};
    std::vector<float> xz_pos_normal = {0, 1, 0};
    std::vector<float> xz_neg_normal = {0, -1, 0};
    std::vector<float> yz_pos_normal = {1, 0, 0};
    std::vector<float> yz_neg_normal = {-1, 0, 0};


    // Setting up XY plane faces of cube

    for (float x = -0.5; x < 0.5; x += incr){
        for (float y = -0.5; y < 0.5; y += incr){

            float z = -0.5;

            // Set up some variables to avoid recalculations
            float x_plus = x + incr;
            if(x_plus > 0.5) {
                x_plus = 0.5;
            }
            float y_plus = y + incr;
            if(y_plus > 0.5) {
                y_plus = 0.5;
            }

            // Bottom XY face

            // First triangle
            m_vhandler->insertTriangleOneNormal(x, x, x_plus, y, y_plus, y_plus, z, z, z, vec, xy_neg_normal);

            // Second triangle
            m_vhandler->insertTriangleOneNormal(x, x_plus, x_plus, y, y_plus, y, z, z, z, vec, xy_neg_normal);

            z = 0.5;

            // Top XY face

            // First triangle
            m_vhandler->insertTriangleOneNormal(x_plus, x, x, y_plus, y_plus, y, z, z, z, vec, xy_pos_normal);

            // Second triangle
            m_vhandler->insertTriangleOneNormal(x_plus, x_plus, x, y, y_plus, y, z, z, z, vec, xy_pos_normal);
        }
    }


    // Setting up XZ plane faces of cube

    for (float x = -0.5; x < 0.5; x += incr){
        for (float z = -0.5; z < 0.5; z += incr) {

            float y = 0.5;

            // Set up some variables to avoid recalculations
            float x_plus = x + incr;
            float z_plus = z + incr;

            if (x_plus > 0.5) {
                x_plus = 0.5;
            }
            if (z_plus > 0.5) {
                z_plus = 0.5;
            }

            // Positive Y

            // First triangle
            m_vhandler->insertTriangleOneNormal(x, x, x_plus, y, y, y, z, z_plus, z_plus, vec, xz_pos_normal);

            // Second triangle
            m_vhandler->insertTriangleOneNormal(x, x_plus, x_plus, y, y, y, z, z_plus, z, vec, xz_pos_normal);

            y = -0.5;

            // Negative Y

            // First triangle
            m_vhandler->insertTriangleOneNormal(x_plus, x, x, y, y, y, z_plus, z_plus, z, vec, xz_neg_normal);

            // Second triangle
            m_vhandler->insertTriangleOneNormal(x_plus, x_plus, x, y, y, y, z, z_plus, z, vec, xz_neg_normal);
        }
    }

    // Setting up the YZ plane faces of the cube
    for (float y = -0.5; y < 0.5; y += incr){
        for (float z = -0.5; z < 0.5; z += incr) {

            float x = -0.5;

            // Set up some variables to avoid recalculations
            float y_plus = y + incr;
            float z_plus = z + incr;

            if (y_plus > 0.5) {
                y_plus = 0.5;
            }
            if (z_plus > 0.5) {
                z_plus = 0.5;
            }

            // Negative x

            // First triangle
            m_vhandler->insertTriangleOneNormal(x, x, x, y, y, y_plus, z, z_plus, z_plus, vec, yz_neg_normal);

            // Second triangle
            m_vhandler->insertTriangleOneNormal(x, x, x, y, y_plus, y_plus, z, z_plus, z, vec, yz_neg_normal);

            x = 0.5;

            // Positive x

            // First triangle
            m_vhandler->insertTriangleOneNormal(x, x, x, y_plus, y, y, z_plus, z_plus, z, vec, yz_pos_normal);

            // Second triangle
            m_vhandler->insertTriangleOneNormal(x, x, x, y_plus, y_plus, y, z, z_plus, z, vec, yz_pos_normal);
        }
    }
    return vec;
}

void Cube::initialize() {
    std::vector<float> vec = getVertices();
    setVertexData(vec.data(), vec.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES, vec.size()/3);
    setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    setAttribute(ShaderAttrib::NORMAL, 3, 12, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    buildVAO();
}



