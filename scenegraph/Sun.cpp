#include "Sun.h"

#include "gl/datatype/VBO.h"
#include <QImage>
#include <iostream>
#include "lib/CubeLoader.h"
#include "gl/shaders/ShaderAttribLocations.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>


Sun::Sun(glm::mat4x4 view, glm::mat4x4 projection, std::vector<std::string> &faces, glm::vec3 translate, glm::vec3 scale, float speed) :
    m_sphereTexture(NULL),
    m_sphere(nullptr),
    m_pMatrix(projection),
    m_vMatrix(view),
    m_translate(translate),
    m_scale(scale),
    m_fps(60.0f),
    m_increment(0),
    m_speed(speed)
{
    m_program = CubeLoader::createShaderProgram(":/shaders/shaders/sun.vert", ":/shaders/shaders/sun.frag");
    m_sphere = std::make_unique<Sphere>();
    m_sphereTexture = load(faces);

    m_lightPositions.resize(6);
    m_lightColors.resize(6);

    m_lightPositions[0] = glm::vec3(0.0f, 0.0f, 100.0f);
    m_lightPositions[1] = glm::vec3(0.0f, 0.0f, -100.0f);
    m_lightPositions[2] = glm::vec3(0.0f, 100.0f, 0.0f);
    m_lightPositions[3] = glm::vec3(0.0f, -100.0f, 0.0f);
    m_lightPositions[4] = glm::vec3(100.0f, 0.0f, 0.0f);
    m_lightPositions[5] = glm::vec3(-100.0f, 0.0f, 0.0f);

    m_lightColors[0] = glm::vec3(236.f, 96.f, 11.f);
    m_lightColors[1] = glm::vec3(242.f, 166.f, 20.f);
    m_lightColors[2] = glm::vec3(241.f, 100.f, 14.f);
    m_lightColors[3] = glm::vec3(230.f, 120.f, 19.f);
    m_lightColors[4] = glm::vec3(245.f, 69.f, 20.f);
    m_lightColors[5] = glm::vec3(245.f, 127.f, 20.f);
}

Sun::~Sun() {
}

void Sun::draw() {
    float time = m_increment++ / (float) m_fps; // Time in seconds.

    glUseProgram(m_program);

    glm::mat4x4 scale = glm::scale(m_scale);
    glm::mat4x4 rotate = glm::rotate(scale, 0 * time, glm::vec3(0, 1, 0));
    glm::mat4x4 translate = glm::translate(rotate, m_translate);

    glUniformMatrix4fv(glGetUniformLocation(m_program, "model"), 1, false, glm::value_ptr(translate));

    glUniformMatrix4fv(glGetUniformLocation(m_program, "projection"), 1, GL_FALSE, glm::value_ptr(m_pMatrix));

    glUniformMatrix4fv(glGetUniformLocation(m_program, "view"), 1, GL_FALSE, glm::value_ptr(m_vMatrix));
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_sphereTexture);

    // Setting the uniforms for light position and color
    for (unsigned int i = 0; i < m_lightPositions.size(); i++) {
        std::string pos = "lights[" + std::to_string(i) + "].position";
        std::string color = "lights[" + std::to_string(i) + "].color";

        glm::vec3 position = m_lightPositions[i];
        glm::vec3 color2 = m_lightColors[i];

        glUniform3fv(glGetUniformLocation(m_program, pos.c_str()),
                     1,
                     glm::value_ptr(position));
        glUniform3fv(glGetUniformLocation(m_program, color.c_str()),
                     1,
                     glm::value_ptr(color2));
    }

    m_sphere->draw();

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glUseProgram(0);
}

unsigned int Sun::load(std::vector<std::string> faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    int width, height;
    QString string = QString();
    for (unsigned int i = 0; i < faces.size(); i++) {
        QString str = string.fromStdString(faces[i]);
        QImage texture = QImage(str);
        width = texture.width();
        height = texture.height();
        unsigned char *data = texture.bits();
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0,  GL_RGBA, width, height, 0,  GL_BGRA, GL_UNSIGNED_BYTE, data);
        } else {
            std::cout << "Planet failed to load for: "<< faces[i] << std::endl;
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void Sun::setProjection(glm::mat4x4 p) {
    m_pMatrix = p;
}

void Sun::setView(glm::mat4x4 v) {
    m_vMatrix = v;
}
