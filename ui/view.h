#ifndef VIEW_H
#define VIEW_H

#include "GL/glew.h"
#include <qgl.h>
#include <QTime>
#include <QTimer>
#include <memory>
#include "camera/OrbitingCamera.h"
#include "camera/BezierCurveUtil.h"
#include "skybox/skybox.h"
#include "scenegraph/Planet.h"
#include "scenegraph/Sun.h"

class View : public QGLWidget
{
    Q_OBJECT

public:
    View(QWidget *parent);
    ~View();

private:
    QTime m_time;
    QTimer m_timer;
    bool m_captureMouse;
    bool m_isDragging;

    // For Bezier curve
    float m_fps;
    int m_increment;
    glm::vec2 m_start;
    glm::vec2 m_end;
    float m_t;
    int m_mainIndex;
    int m_controlIndex;
    std::unique_ptr<BezierCurveUtil> m_bezierUtil;

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void setupBlurBuffer(int w, int h);
    void initializeFBO(int w, int h);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void wheelEvent(QWheelEvent *event);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    void setProjectionAndViewPlanets(glm::mat4x4 projection, glm::mat4x4 view);

    std::unique_ptr<OrbitingCamera> m_camera;
    std::unique_ptr<Skybox> m_skybox;
    std::unique_ptr<Sun> m_sun;
    std::unique_ptr<Planet> m_earth;
    std::unique_ptr<Planet> m_jupiter;
    std::unique_ptr<Planet> m_venus;
    std::unique_ptr<Planet> m_mars;
    std::unique_ptr<Planet> m_mercury;
    std::unique_ptr<Planet> m_saturn;
    std::unique_ptr<Planet> m_uranus;
    std::unique_ptr<Planet> m_neptune;
    std::unique_ptr<Planet> m_pluto;

    // HDR and bloom
    unsigned int m_colorBuffer[2];
    unsigned int m_hdrFBO;
    unsigned int m_pingpongFBO[2];
    unsigned int m_pingpongColorbuffers[2];
    std::vector<glm::vec3> m_lightPositions;
    std::vector<glm::vec3> m_lightColors;
    GLuint m_program;
    GLuint m_hdrProgram;
    GLuint m_blurProgram;
    float m_exposure;

    // XZ coordinates for the planets and sun in stationary mode
    std::vector<glm::vec2> m_bezierMainsXZ = {
        glm::vec2(0.f, -2.f), // sun
        glm::vec2(-1.5f, 0.f), // mercury
        glm::vec2(2.35f, 0.f), // venus
        glm::vec2(-1.5f, 2.3f), // earth
        glm::vec2(2.35f, 2.6f), // mars
        glm::vec2(-2.1f, -4.85f), // jupiter
        glm::vec2(6.5f, -0.1f), // saturn
        glm::vec2(-6.9f, 3.6f), // uranus
        glm::vec2(7.f, -6.6f), // neptune
        glm::vec2(-10.f, 0.f)  // pluto
    };

    // XZ coordinates for the planets and sun in stationary mode
    std::vector<glm::vec4> m_bezierControlsXZ = {
        glm::vec4(-1.8f, 0.3f, -1.6f, -0.3f), // sun - mercury
        glm::vec4(0.f, 0.3f, 1.f, -0.3f), // mercury - venus
        glm::vec4(0.3f, -0.3f, -0.3f, 2.4f), // venus - earth
        glm::vec4(-0.4f, 2.6f, 1.f, 2.f), // earth - mars
        glm::vec4(2.f, -1.f, -1.7f, -1.5f), // mars - jupiter
        glm::vec4(-0.4f, -0.7f, 6.f, -0.9f), // jupiter - saturn
        glm::vec4(4.f, 1.5f, -3.f, 0.2f), // saturn - uranus
        glm::vec4(-0.2f, 3.8f, 0.2f, -5.f), // uranus - neptune
        glm::vec4(2.3f, 0.f, -2.1f, -4.85f), // neptune - pluto // experiment w controls being other planets
        glm::vec4(-1.5f, 2.3f, -1.f, -2.5f)  // pluto - sun
    };

    // All the cube map images for the planets
    std::vector<std::string> m_sunfaces = {
        ":/sun/images/sun /sunright250px.jpg",
        ":/sun/images/sun /sunleft250px.jpg",
        ":/sun/images/sun /suntop250px.jpg",
        ":/sun/images/sun /sunbottom250px.jpg",
        ":/sun/images/sun /sunfront250px.jpg",
        ":/sun/images/sun /sunback250px.jpg"
    };

    std::vector<std::string> m_earthfaces = {
        ":/earth/images/earthright.jpg",
        ":/earth/images/earthleft.jpg",
        ":/earth/images/earthtop.jpg",
        ":/earth/images/earthbottom.jpg",
        ":/earth/images/earthfront.jpg",
        ":/earth/images/earthback.jpg"
    };

    std::vector<std::string> m_jupiterfaces = {
        ":/jupiter/images/rightj.jpg",
        ":/jupiter/images/leftj.jpg",
        ":/jupiter/images/topj.jpg",
        ":/jupiter/images/bottomj.jpg",
        ":/jupiter/images/frontj.jpg",
        ":/jupiter/images/backj.jpg"
    };

    std::vector<std::string> m_venusfaces = {
        ":/venus/images/rightv.jpg",
        ":/venus/images/leftv.jpg",
        ":/venus/images/rightv.jpg",
        ":/venus/images/bottomv.jpg",
        ":/venus/images/frontv.jpg",
        ":/venus/images/backv.jpg"
    };

    std::vector<std::string> m_marsfaces = {
        ":/mars/images/marsright.jpg",
        ":/mars/images/marsleft.jpg",
        ":/mars/images/marstop.jpg",
        ":/mars/images/marsbottom.jpg",
        ":/mars/images/marsfront.jpg",
        ":/mars/images/marsback.jpg"
    };

    std::vector<std::string> m_mercuryfaces = {
        ":/mercury/images/mercuryright.jpg",
        ":/mercury/images/mercuryleft.jpg",
        ":/mercury/images/mercurytop.jpg",
        ":/mercury/images/mercurybottom.jpg",
        ":/mercury/images/mercuryfront.jpg",
        ":/mercury/images/mercuryback.jpg"
    };

    std::vector<std::string> m_neptunefaces = {
        ":/neptune/images/neptuneright.jpg",
        ":/neptune/images/neptuneleft.jpg",
        ":/neptune/images/neptunetop.jpg",
        ":/neptune/images/neptunebottom.jpg",
        ":/neptune/images/neptunefront.jpg",
        ":/neptune/images/neptuneback.jpg"
    };

    std::vector<std::string> m_saturnfaces = {
        ":/saturn/images/saturnright.jpg",
        ":/saturn/images/saturnleft.jpg",
        ":/saturn/images/saturntop.jpg",
        ":/saturn/images/saturnbottom.jpg",
        ":/saturn/images/saturnfront.jpg",
        ":/saturn/images/saturnback.jpg"
    };

    std::vector<std::string> m_uranusfaces = {
        ":/uranus/images/uranusright.jpg",
        ":/uranus/images/uranusleft.jpg",
        ":/uranus/images/uranustop.jpg",
        ":/uranus/images/uranusbottom.jpg",
        ":/uranus/images/uranusfront.jpg",
        ":/uranus/images/uranusback.jpg"
    };

    std::vector<std::string> m_plutofaces = {
        ":/pluto/images/plutoright.jpg",
        ":/pluto/images/plutoleft.jpg",
        ":/pluto/images/plutotop.jpg",
        ":/pluto/images/plutobottom.jpg",
        ":/pluto/images/plutofront.jpg",
        ":/pluto/images/plutoback.jpg"
    };

    std::vector<std::string> m_earthnormals = {
        ":/normals/images/bricknormal.jpg",
        ":/normals/images/bricknormal.jpg",
        ":/normals/images/bricknormal.jpg",
        ":/normals/images/bricknormal.jpg",
        ":/normals/images/bricknormal.jpg",
        ":/normals/images/bricknormal.jpg"
    };



private slots:
    void tick();
};

#endif // VIEW_H
