#include "VertexHandler.h"
#include "Shape.h"

VertexHandler::VertexHandler()
{
}

VertexHandler::~VertexHandler() {

}

/*
 * Function to insert vertices of a triangle
 * x(1-3), y(1-3), z(1-3) - coordinates of each vertex
 * vec - vector to insert into
 * normal(1-3) - the normal for each vertex
 *
 */

void VertexHandler::insertTriangle(float x1, float x2, float x3, float y1, float y2, float y3, float z1, float z2, float z3,
                              std::vector<float> &vec, std::vector<float> normal_1, std::vector<float> normal_2,
                              std::vector<float> normal_3) {

    insertVertexAndNormal(x1, y1, z1, vec, normal_1);

    insertVertexAndNormal(x2, y2, z2, vec, normal_2);

    insertVertexAndNormal(x3, y3, z3, vec, normal_3);

}

/*
 * Function to insert vertices of a triangle
 * x(1-3), y(1-3), z(1-3) - coordinates of each vertex
 * vec - vector to insert into
 * normal - the normal for these vertices
 *
 */

void VertexHandler::insertTriangleOneNormal(float x1, float x2, float x3, float y1, float y2, float y3, float z1, float z2,
                                       float z3, std::vector<float> &vec, std::vector<float> normal) {

    insertVertexAndNormal(x1, y1, z1, vec, normal);

    insertVertexAndNormal(x2, y2, z2, vec, normal);

    insertVertexAndNormal(x3, y3, z3, vec, normal);

}

void VertexHandler::insertTriangleTextureCoords(float x1, float x2, float x3, float y1, float y2, float y3, float z1, float z2, float z3,
                                                std::vector<float> &vec, std::vector<float> normal_1, std::vector<float> normal_2,
                                                std::vector<float> normal_3, std::vector<float> uv1, std::vector<float> uv2,
                                                std::vector<float> uv3, std::vector<float> tangent, std::vector<float> bitangent) {
    insertVertexNormalAndTexture(x1, y1, z1, vec, normal_1, uv1, tangent, bitangent);

    insertVertexNormalAndTexture(x2, y2, z2, vec, normal_2, uv2, tangent, bitangent);

    insertVertexNormalAndTexture(x3, y3, z3, vec, normal_3, uv3, tangent, bitangent);

}

/*
 * Function to insert a vertex and normal into the vector. Creates a temporary vector for the vertex
 * and uses the vector.insert() function to add it onto the end of the vector
 * x, y, z - coordinates of the vertex
 * vec - vector to insert into
 * normal - the normal for that vertex
 *
 */

void VertexHandler::insertVertexAndNormal(float x, float y, float z, std::vector<float> &vec, std::vector<float> normal) {

    std::vector<float> temp = {x, y, z};
    vec.insert(vec.end(), temp.begin(), temp.end());
    vec.insert(vec.end(), normal.begin(), normal.end());

}


/*
 * Function to insert a vertex, a normal, and texture coordinates into the vector. Used for texture mapping
 * in sphere class.
 */

void VertexHandler::insertVertexNormalAndTexture(float x, float y, float z, std::vector<float> &vec, std::vector<float> normal,
                                                 std::vector<float> uv, std::vector<float> tangent, std::vector<float> bitangent) {
    insertVertexAndNormal(x, y, z, vec, normal);
    vec.insert(vec.end(), uv.begin(), uv.end());
    vec.insert(vec.end(), tangent.begin(), tangent.end());
    vec.insert(vec.end(), bitangent.begin(), bitangent.end());
}
