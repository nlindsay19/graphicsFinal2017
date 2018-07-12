#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"

class Sphere : public Shape
{
public:
    Sphere();
    ~Sphere();
    std::vector<float> getVertices() override;
    void initialize() override;
    float findUCurved(float x, float z);
    std::vector<float> findUV(float x, float y, float z);
    std::vector<float> findTangent(glm::vec3 deltaPos1, glm::vec3 deltaPos2, glm::vec2 deltaUV1, glm::vec2 deltaUV2);
    std::vector<float> findBitangent(glm::vec3 deltaPos1, glm::vec3 deltaPos2, glm::vec2 deltaUV1, glm::vec2 deltaUV2);

};

#endif // SPHERE_H
