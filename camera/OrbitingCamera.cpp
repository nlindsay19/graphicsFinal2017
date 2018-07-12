/**
 * @file   OrbitingCamera.cpp
 *
 * (See the header file.) You don't need to be poking around in this file unless you're interested
 * in how an orbiting camera works.
 *
 * The way we have implemented this class is NOT how you should be implementing your Camtrans. This
 * camera is a DIFFERENT TYPE of camera which we're providing so you can easily view your Shapes
 * and to make sure your scene graph is working if your camera isn't.
 *
 * In the Camtrans lab, you'll be implementing your own perspective camera from scratch! This one
 * uses OpenGL.
 */

#include <float.h>
#include <math.h>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

#include "OrbitingCamera.h"
#include "Settings.h"

#include <iostream>


void OrbitingCamera::setAspectRatio(float aspectRatio) {
    m_aspectRatio = aspectRatio;
    updateProjectionMatrix();
}

glm::mat4x4 OrbitingCamera::getProjectionMatrix() const {
    return m_projectionMatrix;
}

glm::mat4x4 OrbitingCamera::getViewMatrix() const {
    return m_viewMatrix;
}

glm::mat4x4 OrbitingCamera::getScaleMatrix() const {
    return m_scaleMatrix;
}

glm::mat4x4 OrbitingCamera::getPerspectiveMatrix() const {
    throw 0; // not implemented
}

void OrbitingCamera::mouseDown(int x, int y) {
    m_oldX = x;
    m_oldY = y;
}

void OrbitingCamera::mouseDragged(int x, int y) {
    m_angleY += x - m_oldX;
    m_angleX += y - m_oldY;
    m_oldX = x;
    m_oldY = y;
    if (m_angleX < -90) m_angleX = -90;
    if (m_angleX > 90) m_angleX = 90;

    updateViewMatrix();
}

void OrbitingCamera::mouseScrolled(int delta) {
    // Use an exponential factor so the zoom increments are small when we are
    // close to the object and large when we are far away from the object
    m_zoomZ *= powf(0.999f, delta);

    updateViewMatrix();
}

void OrbitingCamera::updateMatrices() {
    updateProjectionMatrix();
    updateViewMatrix();
}

void OrbitingCamera::updateProjectionMatrix() {
    m_projectionMatrix = glm::infinitePerspective(glm::radians(100.f), m_aspectRatio, 0.01f);
}

void OrbitingCamera::updateViewMatrix() {

    m_viewMatrix = glm::translate(m_viewTranslation) *
            glm::rotate(glm::radians(m_angleY), glm::vec3(0.f, 1.f, 0.f)) *
            glm::rotate(glm::radians(m_angleX), glm::vec3(1.f, 0.f, 0.f));
}

void OrbitingCamera::setZ(float z) {
    m_zoomZ = z;
}

void OrbitingCamera::setViewTranslation(glm::vec2 translation) {
    m_viewTranslation = glm::vec3(translation, m_zoomZ);
}

glm::vec3 OrbitingCamera::getViewTranslation() {
    return m_viewTranslation;
}

float OrbitingCamera::getZ() {
    return m_zoomZ;
}
