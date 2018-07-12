#ifndef VERTEXHANDLER_H
#define VERTEXHANDLER_H

#include "GL/glew.h"

#include <memory>
#include <vector>

#include "gl/datatype/VBO.h"
#include "gl/datatype/VBOAttribMarker.h"

class VertexHandler
{
public:
    VertexHandler();
    ~VertexHandler();

    void insertTriangle(float x1, float x2, float x3, float y1, float y2, float y3, float z1, float z2, float z3,
                        std::vector<float> &vec, std::vector<float> normal_1, std::vector<float> normal_2,
                        std::vector<float> normal_3);

    void insertTriangleOneNormal(float x1, float x2, float x3, float y1, float y2, float y3, float z1, float z2, float z3,
                        std::vector<float> &vec, std::vector<float> normal_1);

    void insertTriangleTextureCoords(float x1, float x2, float x3, float y1, float y2, float y3, float z1, float z2, float z3,
                        std::vector<float> &vec, std::vector<float> normal_1, std::vector<float> normal_2,
                        std::vector<float> normal_3, std::vector<float> uv1, std::vector<float> uv2, std::vector<float> uv3,
                                     std::vector<float> tangent, std::vector<float> bitangent);

private:
    void insertVertexAndNormal(float x, float y, float z, std::vector<float> &vec,
                        std::vector<float> normal);
    void insertVertexNormalAndTexture(float x, float y, float z, std::vector<float> &vec,
                        std::vector<float> normal, std::vector<float> uv, std::vector<float> tangent,
                                      std::vector<float> bitangent);

};

#endif // VERTEXHANDLER_H
