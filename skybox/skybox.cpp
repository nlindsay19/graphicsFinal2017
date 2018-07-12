#include "skybox.h"
#include "gl/datatype/VBO.h"
#include <QImage>
#include <iostream>
#include "lib/CubeLoader.h"
#include "gl/shaders/ShaderAttribLocations.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "scenegraph/Cube.h"



Skybox::Skybox(glm::mat4x4 view, glm::mat4x4 projection) :
    m_cubeMapTexture(NULL),
    m_view(view),
    m_projection(projection),
    m_box(nullptr)
{
    m_program = CubeLoader::createShaderProgram(":/shaders/shaders/skybox.vert", ":/shaders/shaders/skybox.frag");
    std::vector<std::string> faces = {
        ":/images/images/skyboxsickright.jpg",
        ":/images/images/skyboxsickleft.jpg",
        ":/images/images/skyboxsickfront.jpg",
        ":/images/images/skyboxsickback.jpg",
        ":/images/images/skyboxsickbottom.jpg",
        ":/images/images/skyboxsicktop.jpg"
    };

    m_box = std::make_unique<Cube>(1);
    m_box->setVertexData(&m_skyboxVertices[0], m_skyboxVertices.size(), CS123::GL::VBO::LAYOUT_TRIANGLES, 36);
    m_box->setAttribute(CS123::GL::ShaderAttrib::POSITION, 3, 0, CS123::GL::VBOAttribMarker::DATA_TYPE::FLOAT, false);
    m_box->buildVAO();

    m_cubeMapTexture = load(faces);
}

void Skybox::draw() {
    glDepthMask(GL_FALSE);
    glUseProgram(m_program);
    glUniformMatrix4fv(glGetUniformLocation(m_program, "view"), 1, GL_FALSE, glm::value_ptr(m_view));
    glUniform1i(glGetUniformLocation(m_program, "skybox"), 0);
    glUniformMatrix4fv(glGetUniformLocation(m_program, "projection"), 1, GL_FALSE, glm::value_ptr(m_projection));
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapTexture);

    m_box->draw();

    glDepthMask(GL_TRUE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glUseProgram(0);
}


unsigned int Skybox::load(std::vector<std::string> faces) {
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
            std::cout << "Skybox failed to load for: "<< faces[i] << std::endl;
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void Skybox::setView(glm::mat4x4 view) {
    m_view = view;
}

void Skybox::setProjection(glm::mat4x4 p) {
    m_projection = p;
}
