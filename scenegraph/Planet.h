#ifndef PLANET_H
#define PLANET_H
#include "Sphere.h"

#include <memory>

class Planet
{
public:
    Planet(glm::mat4x4 view, glm::mat4x4 projection, std::vector<std::__cxx11::string> &faces, glm::vec3 translate,
           glm::vec3 scale, float speed, std::__cxx11::string normal);
    ~Planet();
    unsigned int load(std::vector<std::string> faces);
    void draw();
    void setProjection(glm::mat4x4 p);
    void setView(glm::mat4x4 v);
    void setSpeed(float speed);

private:
    GLuint m_program;
    unsigned int m_sphereTexture;
    unsigned int m_sphereNormals;
    std::unique_ptr<Sphere> m_sphere;
    glm::mat4x4 m_pMatrix;
    glm::mat4x4 m_vMatrix;
    //glm::mat4x4 m_translate;
    glm::vec3 m_translate;
    glm::vec3 m_scale;
    float m_fps;


    /** Incremented on every call to paintGL. */
    int m_increment;
    float m_speed;
};

#endif // PLANET_H
