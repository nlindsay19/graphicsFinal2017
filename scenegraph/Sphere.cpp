#include "Sphere.h"
#include "Settings.h"
#include <math.h>
#include "lib/CS123SceneData.h"
#include "glm/gtx/transform.hpp"
#include "lib/CubeLoader.h"
#include "glm/glm.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


Sphere::Sphere() :
    Shape()
{
    std::vector<float> vec = getVertices();
    setVertexData(vec.data(), vec.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES, (vec.size()/14) * 5);
    setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    setAttribute(ShaderAttrib::NORMAL, 3, 12, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    setAttribute(ShaderAttrib::TEXCOORD0, 2, 24, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    setAttribute(ShaderAttrib::TANGENT, 3, 32, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    setAttribute(ShaderAttrib::BINORMAL, 3, 44, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    buildVAO();
}

Sphere::~Sphere() {
}

/*
 * This function calculates and inserts all the vertices for the shape. It uses the VertexHandler
 * object to insert each triangle, passing the coordinates of each vertex into its insertTriangle
 * function.
 */

std::vector<float> Sphere::getVertices() {

    // Getting tesselation parameters
    int p1 = 60;
    int p2 = 60;

    // Checking value to make sure shape is possible
    if (p1 < 2) {
        p1 = 2;
    }

    if (p2 < 3) {
        p2 = 3;
    }

    // Setting up varables for 2Pi, the increments, and the radiusfind tangent based on normal and posiiton
    float two_pi = 2 * M_PI;
    float theta_incr = two_pi / p2;
    float phi_incr = M_PI / p1;
    float r = 0.5;

    // Setting up vertex vector and the normals, which will be instantiated in the loop
    std::vector<float> vec = {};
    std::vector<float> normal;
    std::vector<float> normal_theta_plus;
    std::vector<float> normal_phi_plus;
    std::vector<float> normal_both_plus;

    for (float theta = 0; theta < two_pi; theta += theta_incr) {

        // Getting the next theta value for vertex calculation
        float theta_plus = theta + theta_incr;
        if (theta_plus > two_pi) {
            theta_plus = two_pi;
        }

        for (float phi = 0; phi < M_PI; phi += phi_incr) {

            // Getting the next phi value for vertex calcuation
            float phi_plus = phi + phi_incr;
            if (phi_plus > M_PI) {
                phi_plus = M_PI;
            }

            // The normal for the current phi and theta, and then the normals for the next incremental phi and theta
            normal = {sin(phi) * cos(theta), cos(phi), sin(phi) * sin(theta)};
            normal_theta_plus = {sin(phi) * cos(theta_plus), cos(phi), sin(phi) * sin(theta_plus)};
            normal_phi_plus = {sin(phi_plus) * cos(theta), cos(phi_plus), sin(phi_plus) * sin(theta)};
            normal_both_plus = {sin(phi_plus) * cos(theta_plus), cos(phi_plus), sin(phi_plus) * sin(theta_plus)};

            // Setting up variables to avoid recalculations
            float x = r * sin(phi) * cos(theta);
            float y = r * cos(phi);
            float z = r * sin(phi) * sin(theta);
            float x_theta_plus = r * sin(phi) * cos(theta_plus);
            float y_phi_plus = r * cos(phi_plus);
            float z_theta_plus = r * sin(phi) * sin(theta_plus);
            float x_phi_plus = r * sin(phi_plus) * cos(theta);
            float z_phi_plus = r * sin(phi_plus) * sin(theta);
            float x_phi_theta_plus = r * sin(phi_plus) * cos(theta_plus);
            float z_phi_theta_plus = r * sin(phi_plus) * sin(theta_plus);

            // Find uv coords for texture mapping
            std::vector<float> uvCoords1 = findUV(x, y, z);
            std::vector<float> uvCoords2 = findUV(x_theta_plus, y, z_theta_plus);
            std::vector<float> uvCoords3 = findUV(x_phi_theta_plus, y_phi_plus, z_phi_theta_plus);

            // Find tangent and bitangent vectors using uv coordinates
            glm::vec3 deltaPos1 = glm::vec3(x_theta_plus - x, y - y, z_theta_plus - z);
            glm::vec3 deltaPos2 = glm::vec3(x_phi_theta_plus - x, y_phi_plus - y, z_phi_theta_plus - z);
            glm::vec2 deltaUV1 = glm::vec2(uvCoords2[0] - uvCoords1[0], uvCoords2[1] - uvCoords1[1]);
            glm::vec2 deltaUV2 = glm::vec2(uvCoords3[0] - uvCoords1[0], uvCoords3[1] - uvCoords1[1]);

            std::vector<float> tangent = findTangent(deltaPos1, deltaPos2, deltaUV1, deltaUV2);
            std::vector<float> bitangent = findBitangent(deltaPos1, deltaPos2, deltaUV1, deltaUV2);

            // First triangle
            if (phi != 0) {
//                m_vhandler->insertTriangle(x, x_theta_plus, x_phi_theta_plus, y, y, y_phi_plus, z, z_theta_plus, z_phi_theta_plus,
//                                           vec, normal, normal_theta_plus, normal_both_plus);
                m_vhandler->insertTriangleTextureCoords(x, x_theta_plus, x_phi_theta_plus, y, y, y_phi_plus, z, z_theta_plus, z_phi_theta_plus,
                                                        vec, normal, normal_theta_plus, normal_both_plus, uvCoords1, uvCoords2, uvCoords3,
                                                        tangent, bitangent);
            }

            uvCoords1 = findUV(x, y, z);
            uvCoords2 = findUV(x_phi_theta_plus, y_phi_plus, z_phi_theta_plus);
            uvCoords3 = findUV(x_phi_plus, y_phi_plus, z_phi_plus);

            deltaPos1 = glm::vec3(x_phi_theta_plus - x, y_phi_plus - y, z_phi_theta_plus - z);
            deltaPos2 = glm::vec3(x_phi_plus - x, y_phi_plus - y, z_phi_plus - z);
            deltaUV1 = glm::vec2(uvCoords2[0] - uvCoords1[0], uvCoords2[1] - uvCoords1[1]);
            deltaUV2 = glm::vec2(uvCoords3[0] - uvCoords1[0], uvCoords3[1] - uvCoords1[1]);

            tangent = findTangent(deltaPos1, deltaPos2, deltaUV1, deltaUV2);
            bitangent = findBitangent(deltaPos1, deltaPos2, deltaUV1, deltaUV2);

            // Second triangle
//            m_vhandler->insertTriangle(x, x_phi_theta_plus, x_phi_plus, y, y_phi_plus, y_phi_plus, z, z_phi_theta_plus, z_phi_plus,
//                                       vec, normal, normal_both_plus, normal_phi_plus);
            m_vhandler->insertTriangleTextureCoords(x, x_phi_theta_plus, x_phi_plus, y, y_phi_plus, y_phi_plus, z, z_phi_theta_plus, z_phi_plus,
                                       vec, normal, normal_both_plus, normal_phi_plus, uvCoords1, uvCoords2, uvCoords3,
                                                    tangent, bitangent);

        }
    }

    return vec;

}

void Sphere::initialize() {
    std::vector<float> vec = getVertices();
    setVertexData(vec.data(), vec.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLES, vec.size()/3);
    setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    setAttribute(ShaderAttrib::NORMAL, 3, 12, VBOAttribMarker::DATA_TYPE::FLOAT, false);
    buildVAO();

}

std::vector<float> Sphere::findUV(float x, float y, float z) {
    float u;
    if (std::abs(y - 0.5f) < 0.001f || std::abs(y + 0.5f) < 0.001f) {
        u = 0.5f;
    } else {
        u = findUCurved(x, z);
    }

    float v = asin(y / 0.5f) / M_PI;
    std::vector<float> uvCoords = {u, 1.f - (v + 0.5f)};
    return uvCoords;
}

float Sphere::findUCurved(float x, float z) {
    float theta = atan2(z, x);

    if (theta < 0) {
        return (-theta / (2.f*M_PI));
    } else {
        return (1 - (theta / (2.f*M_PI)));
    }
}

std::vector<float> Sphere::findTangent(glm::vec3 deltaPos1, glm::vec3 deltaPos2, glm::vec2 deltaUV1, glm::vec2 deltaUV2) {
    float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
    glm::vec3 tangent = (deltaPos1 * deltaUV2.y   - deltaPos2 * deltaUV1.y)*r;
    std::vector<float> tan = {tangent[0], tangent[1], tangent[2]};
    return tan;

}

std::vector<float> Sphere::findBitangent(glm::vec3 deltaPos1, glm::vec3 deltaPos2, glm::vec2 deltaUV1, glm::vec2 deltaUV2) {
    float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
    glm::vec3 bitangent = (deltaPos2 * deltaUV1.x   - deltaPos1 * deltaUV2.x)*r;
    std::vector<float> bitan = {bitangent[0], bitangent[1], bitangent[2]};
    return bitan;
}


