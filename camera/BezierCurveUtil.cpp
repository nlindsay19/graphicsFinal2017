#include "BezierCurveUtil.h"


BezierCurveUtil::BezierCurveUtil()
{
}

BezierCurveUtil::~BezierCurveUtil() {

}

glm::vec2 BezierCurveUtil::travelBetweenTwoPts(float t, glm::vec2 first, glm::vec2 second, glm::vec4 control) {
    // simple linear interpolation
//    float x = first.x + (second.x - first.x)*t;
//    float z = first.y + (second.y - first.y)*t;

    // bezier curve
    float x = ((1.f - t) * (1.f - t) * (1.f - t)) * first.x
            + 3.f * ((1.f - t) * (1.f - t)) * t * control.x
            + 3.f * (1.f - t) * (t * t) * control.z
            + (t * t * t) * second.x;
    float z = ((1.f - t) * (1.f - t) * (1.f - t)) * first.y
            + 3.f * ((1.f - t) * (1.f - t)) * t * control.y
            + 3.f * (1.f - t) * (t * t) * control.w
            + (t * t * t) * second.y;
    return glm::vec2(x, z);
}
