#include "Planet.h"
#include "gl/datatype/VBO.h"
#include <QImage>
#include <iostream>
#include "lib/CubeLoader.h"
#include "gl/shaders/ShaderAttribLocations.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "Settings.h"

Planet::Planet(glm::mat4x4 view, glm::mat4x4 projection, std::vector<std::string> &faces, glm::vec3 translate,
               glm::vec3 scale, float speed, std::string normal) :

    m_sphereTexture(),
    m_sphereNormals(),
    m_sphere(nullptr),
    m_pMatrix(projection),
    m_vMatrix(view),
    m_translate(translate),
    m_scale(scale),
    m_fps(60.0f),
    m_increment(0),
    m_speed(speed)
{
    m_program = CubeLoader::createShaderProgram(":/shaders/shaders/sphere.vert", ":/shaders/shaders/sphere.frag");
    m_sphere = std::make_unique<Sphere>();
    m_sphereTexture = load(faces);

    if (!normal.empty()) {
        // Load the normal map texture
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        QString string = QString();
        QString str = string.fromStdString(normal);
        QImage texture = QImage(str);
        int width = texture.width();
        int height = texture.height();
        unsigned char *data = texture.bits();
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0,  GL_RGBA, width, height, 0,  GL_BGRA, GL_UNSIGNED_BYTE, data);
        } else {
            std::cout << "Normal texture failed to load for: "<< normal << std::endl;
        }
        m_sphereNormals = textureID;
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    } else {
        m_sphereNormals = -1;
    }

}

Planet::~Planet() {

}

void Planet::draw() {

    std::vector<glm::vec3> lightPositions;
    lightPositions.push_back(glm::vec3(0.0f, 0.5f, 0.0f));
    lightPositions.push_back(glm::vec3(0.0f, -0.5f, 0.0f));
    lightPositions.push_back(glm::vec3(0.5f, 0.0f, 0.0f));
    lightPositions.push_back(glm::vec3(-0.5f, 0.0f, 0.0f));
    lightPositions.push_back(glm::vec3(-7.f, 0.f, 8.f)); // neptune light
    std::vector<glm::vec3> lightColors;
    lightColors.push_back(glm::vec3(255.f, 255.f, 204.f));
    lightColors.push_back(glm::vec3(255.f, 255.f, 204.f));
    lightColors.push_back(glm::vec3(255.f, 255.f, 204.f));
    lightColors.push_back(glm::vec3(255.f, 255.f, 204.f));
    lightColors.push_back(glm::vec3(255.f, 255.f, 204.f));

    float time = m_increment++ / (float) m_fps;      // Time in seconds.

    glUseProgram(m_program);

    glm::mat4x4 scale = glm::scale(m_scale);
    glm::mat4x4 rotate1 = glm::rotate(scale, m_speed * time, glm::vec3(0, 1, 0));
    glm::mat4x4 translate = glm::translate(rotate1, m_translate);

    glm::mat4x4 rotate = glm::rotate(translate, (float)3.14/8 * time, glm::vec3(0, 1, 0)); // (float)3.14/8 * time

    glUniformMatrix4fv(glGetUniformLocation(m_program, "model"), 1, false, glm::value_ptr(rotate));

    glUniformMatrix4fv(glGetUniformLocation(m_program, "projection"), 1, GL_FALSE, glm::value_ptr(m_pMatrix));

    glUniformMatrix4fv(glGetUniformLocation(m_program, "view"), 1, GL_FALSE, glm::value_ptr(m_vMatrix));

    // Uniform indicating whether bump mapping is turned on
    int bumpMapOn = (m_sphereNormals == -1 || !settings.bumpMapping) ? 0 : 1;
    glUniform1i(glGetUniformLocation(m_program, "ifBump"), bumpMapOn);

    int bezier = (settings.space_radio == BEZIER) ? 1 : 0;
    glUniform1i(glGetUniformLocation(m_program, "bezier"), bezier);

    // Uniform indicating whether lighting is turned on
    int useLighting = (settings.lighting) ? 1 : 0;
    glUniform1i(glGetUniformLocation(m_program, "useLighting"), useLighting);

    // Bind texture mapping texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_sphereTexture);
    glUniform1i(glGetUniformLocation(m_program, "planet"), 0);

    // Bind normal mapping texture if bump mapping is on
    if (bumpMapOn == 1) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_sphereNormals);
        glUniform1i(glGetUniformLocation(m_program, "normals"), 1);
    }

    // Setting the uniforms for light position and color
    for (unsigned int i = 0; i < lightPositions.size(); i++) {
        std::string pos = "lights[" + std::to_string(i) + "].position";
        std::string color = "lights[" + std::to_string(i) + "].color";

        glm::vec3 position = lightPositions[i];
        glm::vec3 color2 = lightColors[i];

        glUniform3fv(glGetUniformLocation(m_program, pos.c_str()),
                     1,
                     glm::value_ptr(position));
        glUniform3fv(glGetUniformLocation(m_program, color.c_str()),
                     1,
                     glm::value_ptr(color2));
    }

    m_sphere->draw();

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}

unsigned int Planet::load(std::vector<std::string> faces) {
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

void Planet::setProjection(glm::mat4x4 p) {
    m_pMatrix = p;
}

void Planet::setView(glm::mat4x4 v) {
    m_vMatrix = v;

}

void Planet::setSpeed(float speed) {
    m_speed = speed;

}

