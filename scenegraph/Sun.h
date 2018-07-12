#ifndef SUN_H
#define SUN_H

#include "Sphere.h"
#include <glm/glm.hpp>

#include <memory>

class Sun
{
public:
    Sun(glm::mat4x4 view, glm::mat4x4 projection, std::vector<std::__cxx11::string> &faces, glm::vec3 translate,
           glm::vec3 scale, float speed);
    ~Sun();
    unsigned int load(std::vector<std::string> faces);
    void draw();
    void setProjection(glm::mat4x4 p);
    void setView(glm::mat4x4 v);

private:
    GLuint m_program;
    unsigned int m_sphereTexture;
    std::unique_ptr<Sphere> m_sphere;
    glm::mat4x4 m_pMatrix;
    glm::mat4x4 m_vMatrix;
    //glm::mat4x4 m_translate;
    glm::vec3 m_translate;
    glm::vec3 m_scale;
    float m_fps;
    float m_speed;

    /** Incremented on every call to paintGL. */
    int m_increment;

    std::vector<glm::vec3> m_lightPositions;
    std::vector<glm::vec3> m_lightColors;
};

#endif // SUN_H
