#include "view.h"

#include "viewformat.h"
#include <QApplication>
#include <QKeyEvent>
#include <iostream>
#include <memory>
#include "skybox/skybox.h"
#include <QList>
#include "Settings.h"
#include "lib/CubeLoader.h"
#include <glm/gtc/type_ptr.hpp>
#include "gl/util/FullScreenQuad.h"

View::View(QWidget *parent) : QGLWidget(ViewFormat(), parent),
    m_time(), m_timer(), m_captureMouse(false), m_isDragging(false), m_camera(),
    m_fps(60.0f),
    m_increment(0), m_start(glm::vec2(0.f, -2.f)), m_end(glm::vec2(-1.5f, 0.f)), m_t(0.f),
    m_mainIndex(0), m_controlIndex(0),
    m_hdrFBO(), m_pingpongFBO(), m_pingpongColorbuffers(), m_colorBuffer(), m_exposure(1.f)
{
    m_camera = std::make_unique<OrbitingCamera>();
    m_bezierUtil = std::make_unique<BezierCurveUtil>();
    // View needs all mouse move events, not just mouse drag events
    setMouseTracking(true);

    // Hide the cursor
    if(m_captureMouse) {
        QApplication::setOverrideCursor(Qt::BlankCursor);
    }

    // View needs keyboard focus
    setFocusPolicy(Qt::StrongFocus);

    // The update loop is implemented using a timer
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(tick()));
}

View::~View()
{
    glDeleteFramebuffers(1, &m_hdrFBO);
    glDeleteFramebuffers(2, m_pingpongFBO);
}

void View::initializeGL()
{
    // All OpenGL initialization *MUST* be done during or after this
    // method. Before this method is called, there is no active OpenGL
    // context and all OpenGL calls have no effect.

    //initialize glew
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if ( GLEW_OK != err ) {
        /* Problem: glewInit failed, something is seriously wrong. */
        std::cerr << "Something is very wrong, glew initialization failed." << std::endl;
    }

    std::cout << "Using GLEW " <<  glewGetString( GLEW_VERSION ) << std::endl;

    // Start a timer that will try to get 60 frames per second (the actual
    // frame rate depends on the operating system and other running programs)
    m_time.start();
    m_timer.start(1000 / 60);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    if (settings.space_radio == ORBITING || settings.space_radio == STATIONARY) {
        m_camera->setViewTranslation(glm::vec2(0.f, 0.f));
    } else {
        // Sun
        m_camera->setZ(-2.f);
        m_camera->setViewTranslation(glm::vec2(0.f, 0.f));
    }

    m_camera->updateMatrices();

    glm::mat4x4 view = m_camera->getViewMatrix();
    glm::mat4x4 projection = m_camera->getProjectionMatrix();

    m_hdrProgram = CubeLoader::createShaderProgram(":/shaders/shaders/hdr.vert", ":/shaders/shaders/hdr.frag");
    m_blurProgram = CubeLoader::createShaderProgram(":/shaders/shaders/blur.vert", ":/shaders/shaders/blur.frag");

    // Skybox
    m_skybox = std::make_unique<Skybox>(glm::mat4x4(glm::mat3x3(view)), projection);

    // Sun
    m_sun = std::make_unique<Sun>(projection, view, m_sunfaces, glm::vec3(0, 0, 0.f), glm::vec3(2.5f, 2.5f, 2.5f), (float)3.14/6);

    // Planets
    m_mercury = std::make_unique<Planet>(projection, view, m_mercuryfaces, glm::vec3(8.f, 0, -1.f), glm::vec3(0.2f, 0.2f, 0.2f), (float)3.14/8, ":/normals/images/randomnormalmap.jpg");
    m_venus = std::make_unique<Planet>(projection, view, m_venusfaces, glm::vec3(-8.f, 0, -1.f), glm::vec3(0.3f, 0.3f, 0.3f), (float)3.14/12, ":/normals/images/funky.jpg");
    m_earth = std::make_unique<Planet>(projection, view, m_earthfaces, glm::vec3(5.f, 0.f, -8.6f), glm::vec3(0.3f, 0.3f, 0.3f), (float)3.14/12, ":/normals/images/betterearthnormal.jpg");
    m_mars = std::make_unique<Planet>(projection, view, m_marsfaces, glm::vec3(-9.f, 0, -10.7f), glm::vec3(0.27f, 0.27f, 0.27f), (float)3.14/32, ":/normals/images/marsnormal.jpg");
    m_jupiter = std::make_unique<Planet>(projection, view, m_jupiterfaces, glm::vec3(3.f, 0, 4.12f), glm::vec3(1.f, 1.f, 1.f), (float)3.14/48, ":/normals/images/fillernormal.jpg");
    m_saturn = std::make_unique<Planet>(projection, view, m_saturnfaces, glm::vec3(-7.5f, 0, -1.f), glm::vec3(0.9f, 0.9f, 0.9f), (float)3.14/54, ":/normals/images/fillernormal.jpg");
    m_uranus = std::make_unique<Planet>(projection, view, m_uranusfaces, glm::vec3(15.59f, 0, -9.f), glm::vec3(0.45f, 0.45f, 0.45f), (float)3.14/64, ":/normals/images/fillernormal.jpg");
    m_neptune = std::make_unique<Planet>(projection, view, m_neptunefaces, glm::vec3(-13.42f, 0, 12.f), glm::vec3(0.5f, 0.5f, 0.5f), (float)3.14/80, ":/normals/images/fillernormal.jpg");
    m_pluto = std::make_unique<Planet>(projection, view, m_plutofaces, glm::vec3(50.f, 0, -1.f), glm::vec3(0.2f, 0.2f, 0.2f), (float)3.14/100, ":/normals/images/fillernormal.jpg");
}


void View::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set the camera's position for Bezier curves
    if (settings.space_radio != BEZIER) {
        m_mainIndex = 0;
        m_controlIndex = 0;
    }
    if (settings.space_radio == BEZIER) {
        // set up start and end points
        if (m_t > 1.002f) {
            m_t = 0.f;
            m_mainIndex +=1;
            m_controlIndex += 1;
            if (m_mainIndex == m_bezierMainsXZ.size()) {
                m_mainIndex = 0;
                m_controlIndex = 0;
                m_start = m_bezierMainsXZ.at(0);
            } else {
                m_start = m_bezierMainsXZ.at(m_mainIndex);
            }
            if (m_mainIndex == m_bezierMainsXZ.size() - 1) {
                m_end = m_bezierMainsXZ.at(0);
            } else {
                m_end = m_bezierMainsXZ.at(m_mainIndex + 1);
            }
        }
        if (m_t < 0.005f) {
            m_camera->setZ(m_start.y);
            m_camera->setViewTranslation(glm::vec2(m_start.x, 0.f));
            m_t += 0.00005f;
        } else if (m_t == 1.f) {
            m_camera->setZ(m_end.y);
            m_camera->setViewTranslation(glm::vec2(m_end.x, 0.f));
            m_t += 0.0005f;
        } else {
            glm::vec2 out =
                    m_bezierUtil->travelBetweenTwoPts(m_t, m_start, m_end, m_bezierControlsXZ.at(m_controlIndex));
            m_camera->setZ(out.y);
            m_camera->setViewTranslation(glm::vec2(out.x, 0.f));
            m_t += 0.005f;
        }
    }

    m_camera->updateMatrices();
    glm::mat4x4 view = m_camera->getViewMatrix();
    glm::mat4x4 projection = m_camera->getProjectionMatrix();
    m_skybox->setView(glm::mat4x4(glm::mat3x3(view)));
    m_skybox->setProjection(projection);

    m_sun->setProjection(projection);
    m_sun->setView(view);

    setProjectionAndViewPlanets(projection, view);

    if (settings.space_radio == ORBITING) {
        m_mercury->setSpeed((float)3.14/8);
        m_venus->setSpeed((float)3.14/12);
        m_earth->setSpeed((float)3.14/16);
        m_mars->setSpeed((float)3.14/32);
        m_jupiter->setSpeed((float)3.14/48);
        m_saturn->setSpeed((float)3.14/54);
        m_uranus->setSpeed((float)3.14/64);
        m_neptune->setSpeed((float)3.14/80);
        m_pluto->setSpeed((float)3.14/100);
    } else if (settings.space_radio == STATIONARY || settings.space_radio == BEZIER) {
        m_mercury->setSpeed(0.f);
        m_venus->setSpeed(0.f);
        m_earth->setSpeed(0.f);
        m_mars->setSpeed(0.f);
        m_jupiter->setSpeed(0.f);
        m_saturn->setSpeed(0.f);
        m_uranus->setSpeed(0.f);
        m_neptune->setSpeed(0.f);
        m_pluto->setSpeed(0.f);
    }

    if (settings.hdr) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_hdrFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
    }
    m_skybox->draw();
    m_sun->draw();
    m_earth->draw();
    m_jupiter->draw();
    m_venus->draw();
    m_mars->draw();
    m_mercury->draw();
    m_neptune->draw();
    m_saturn->draw();
    m_uranus->draw();
    m_pluto->draw();

    if (settings.hdr) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glUseProgram(m_blurProgram);
        glUniform1i(glGetUniformLocation(m_blurProgram, "image"), 0);
        glActiveTexture(GL_TEXTURE0);
        CS123::GL::FullScreenQuad quad = FullScreenQuad();

        bool horizontal = true;
        bool first_iteration = true;
        unsigned int amount = 10;
        for (unsigned int i = 0; i < amount; i++) {
            glBindFramebuffer(GL_FRAMEBUFFER, m_pingpongFBO[horizontal]);
            glUniform1i(glGetUniformLocation(m_blurProgram, "horizontal"), horizontal);
            glBindTexture(GL_TEXTURE_2D, first_iteration ? m_colorBuffer[1] : m_pingpongColorbuffers[!horizontal]);
            quad.draw();
            horizontal = !horizontal;
            if (first_iteration) {
                first_iteration = false;
            }
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glUseProgram(m_hdrProgram);
        glUniform1i(glGetUniformLocation(m_hdrProgram, "hdrBuffer"), 0);
        glUniform1i(glGetUniformLocation(m_hdrProgram, "bloomBlur"), 1);
        glUniform1f(glGetUniformLocation(m_hdrProgram, "exposure"), m_exposure);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_colorBuffer[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_pingpongColorbuffers[!horizontal]);
        quad.draw();
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    glUseProgram(0);
}

void View::setProjectionAndViewPlanets(glm::mat4x4 projection, glm::mat4x4 view) {
    m_earth->setProjection(projection);
    m_earth->setView(view);

    m_jupiter->setProjection(projection);
    m_jupiter->setView(view);

    m_venus->setProjection(projection);
    m_venus->setView(view);

    m_mars->setProjection(projection);
    m_mars->setView(view);

    m_mercury->setProjection(projection);
    m_mercury->setView(view);

    m_neptune->setProjection(projection);
    m_neptune->setView(view);

    m_saturn->setProjection(projection);
    m_saturn->setView(view);

    m_uranus->setProjection(projection);
    m_uranus->setView(view);

    m_pluto->setProjection(projection);
    m_pluto->setView(view);
}

void View::resizeGL(int w, int h)
{
    initializeFBO(w, h);
    setupBlurBuffer(w, h);
    float ratio = static_cast<QGuiApplication *>(QCoreApplication::instance())->devicePixelRatio();
    m_camera->setAspectRatio(static_cast<float>(width()) / static_cast<float>(height()));
    w = static_cast<int>(w / ratio);
    h = static_cast<int>(h / ratio);
    glViewport(0, 0, w, h);
}

void View::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        m_camera->mouseDown(event->x(), event->y());
        m_isDragging = true;
        update();
    }
}

void View::mouseMoveEvent(QMouseEvent *event) {
    if (m_isDragging) {
        m_camera->mouseDragged(event->x(), event->y());
        update();
    }
}

void View::mouseReleaseEvent(QMouseEvent *event) {
    if (m_isDragging && event->button() == Qt::RightButton) {
        m_camera->mouseUp(event->x(), event->y());
        m_isDragging = false;
        update();
    }
}

void View::wheelEvent(QWheelEvent *event) {
    m_camera->mouseScrolled(event->delta());
    if (settings.space_radio == ORBITING || settings.space_radio == STATIONARY) {
        if (m_camera->getZ() == 0.f) {
            m_camera->setZ(-3.f);
        }
        m_camera->setViewTranslation(glm::vec2(0.f, 0.f));
    }
    update();
}

void View::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) QApplication::quit();
}

void View::keyReleaseEvent(QKeyEvent *event)
{
}

void View::tick()
{
    // Get the number of seconds since the last tick (variable update rate)
    m_time.restart() * 0.001f;
    // Flag this view for repainting (Qt will call paintGL() soon after)
    update();
}

void View::setupBlurBuffer(int w, int h) {

    glGenFramebuffers(2, m_pingpongFBO);
    glGenTextures(2, m_pingpongColorbuffers);
    for (unsigned int i = 0; i < 2; i++) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, m_pingpongColorbuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, w, h, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                               m_pingpongColorbuffers[i], 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout <<"Framebuffer not complete!" << std::endl;
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void View::initializeFBO(int w, int h) {
    glGenFramebuffers(1, &m_hdrFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_hdrFBO);
    glGenTextures(2, m_colorBuffer);
    for (unsigned int i = 0; i < 2; i++) {
        glBindTexture(GL_TEXTURE_2D, m_colorBuffer[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, w, h, 0, GL_RGBA, GL_FLOAT, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D,
                               m_colorBuffer[i], 0);

    }
    unsigned int rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout <<"Framebuffer not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
