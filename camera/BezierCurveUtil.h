#ifndef BEZIERCURVEUTIL_H
#define BEZIERCURVEUTIL_H

#include <vector>
#include <glm/glm.hpp>

class BezierCurveUtil
{
public:
    BezierCurveUtil();
    ~BezierCurveUtil();
    glm::vec2 travelBetweenTwoPts(float t, glm::vec2 first, glm::vec2 second, glm::vec4 control);
};

#endif // BEZIERCURVEUTIL_H
