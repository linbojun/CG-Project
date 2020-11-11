/**
 * @file   CamtransCamera.cpp
 *
 * This is the perspective camera class you will need to fill in for the Camtrans lab.  See the
 * lab handout for more details.
 */
#define cot(x)  1/glm::tan(x)

#include "CamtransCamera.h"
#include <Settings.h>
#include <iostream>

CamtransCamera::CamtransCamera()
{
    // @TODO: [CAMTRANS] Fill this in...
    m_near = 1;
    m_far = 30;
    m_thetaH = 60;
    m_aspectRatio = 1;
    m_eye = glm::vec4(2, 2, 2, 1);
    glm::vec4 look = glm::vec4(0, 0, 0, 0) - m_eye;

    m_w = -glm::normalize(look);
    m_w[3] = 1;
    m_up = glm::vec4(0,1,0,0);
}

void CamtransCamera::setAspectRatio(float a)
{
    // @TODO: [CAMTRANS] Fill this in...
    m_aspectRatio = a;
    float h_rad = m_thetaH;
    float h = glm::tan(h_rad);
    float w = m_aspectRatio * h;
    float w_rad = glm::atan(w);

    m_thetaW = w_rad;

    updateProjectionMatrix();

}

glm::mat4x4 CamtransCamera::getProjectionMatrix() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_perspectiveTransformation * m_scaleMatrix;
}

glm::mat4x4 CamtransCamera::getViewMatrix() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_rotationMatrix * m_translationMatrix;
}

glm::mat4x4 CamtransCamera::getScaleMatrix() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_scaleMatrix;
}

glm::mat4x4 CamtransCamera::getPerspectiveMatrix() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_perspectiveTransformation;
}

glm::vec4 CamtransCamera::getPosition() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_eye;
}

glm::vec4 CamtransCamera::getLook() const {
    // @TODO: [CAMTRANS] Fill this in...
    return -m_w;
}

glm::vec4 CamtransCamera::getUp() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_up;
}

float CamtransCamera::getAspectRatio() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_aspectRatio;
}

float CamtransCamera::getHeightAngle() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_thetaH;
}

void CamtransCamera::orientLook(const glm::vec4 &eye, const glm::vec4 &look, const glm::vec4 &up) {
    // @TODO: [CAMTRANS] Fill this in...
    std::cout<<"Call orientLook "<<std::endl;
    m_eye = eye;
    m_up = up;
    m_w = glm::normalize(-look);
    glm::vec3 _w = glm::vec3(m_w);
    glm::vec3 _up = glm::vec3(up);
    glm::vec3 _v = _up - glm::dot(_up, _w) * _w;
    _v = glm::normalize(_v);
    glm::vec3 _u = glm::cross(_v, _w);
    m_v = glm::vec4(_v, 1);
    m_u = glm::vec4(_u, 1);

    updateViewMatrix();
    updateProjectionMatrix();


}

void CamtransCamera::setHeightAngle(float h_deg) {
    // @TODO: [CAMTRANS] Fill this in...
    m_thetaH = glm::radians(h_deg);
    float h_rad = m_thetaH;
    float h = glm::tan(h_rad);
    float w = m_aspectRatio * h;
    float w_rad = glm::atan(w);

    m_thetaW = w_rad;
    updateProjectionMatrix();
}

void CamtransCamera::translate(const glm::vec4 &v) {
    // @TODO: [CAMTRANS] Fill this in...
    std::cout<<"Call translate "<<std::endl;

    m_eye = m_eye + v;
    m_eye[3] = 0;
    updateViewMatrix();

}

void CamtransCamera::rotateU(float degrees) {
    // @TODO: [CAMTRANS] Fill this in...
    float rad = glm::radians(degrees);

   // float vx = m_v[0], vy = m_v[1], vz = m_v[2];
   // float wx = m_w[0], wy = m_w[1], wz = m_w[2];
    glm::vec3 v0 = glm::vec3(m_v);
    glm::vec3 w0 = glm::vec3(m_w);
    glm::vec3 v = w0 * glm::sin(rad) + v0 * glm::cos(rad);
    glm::vec3 w = w0 * glm::cos(rad) - v0 * glm::sin(rad);
    m_v = glm::vec4(v, 1);
    m_w = glm::vec4(w, 1);
   // m_v[0] = v[0];
   // m_v[1] = v[1];
   // m_v[2] = v[2];

    //m_w[0] = w[0];
    //m_w[1] = w[1];
    //m_w[2] = w[2];
    updateViewMatrix();


}

void CamtransCamera::rotateV(float degrees) {
    // @TODO: [CAMTRANS] Fill this in...
    float rad = glm::radians(degrees);

    //float ux = m_u[0], uy = m_u[1], uz = m_u[2];
    //float wx = m_w[0], wy = m_w[1], wz = m_w[2];
    glm::vec3 u0 = glm::vec3(m_u);
    glm::vec3 w0 = glm::vec3(m_w);
    glm::vec3 u = u0 * glm::cos(rad) -  w0 * glm::sin(rad);
    glm::vec3 w = u0 * glm::sin(rad) +  w0 * glm::cos(rad);
    m_u = glm::vec4(u, 1);
    m_w = glm::vec4(w, 1);

 //   m_u[0] = u[0];
 //   m_u[1] = u[1];
 //   m_u[2] = u[2];

   // m_w[0] = w[0];
   // m_w[1] = w[1];
   // m_w[2] = w[2];
    updateViewMatrix();

}

void CamtransCamera::rotateW(float degrees) {
    // @TODO: [CAMTRANS] Fill this in...
    float rad = glm::radians(degrees);

   // float ux = m_u[0], uy = m_u[1], uz = m_u[2];
   // float vx = m_v[0], vy = m_v[1], vz = m_v[2];
    glm::vec3 u0 = glm::vec3(m_u);
    glm::vec3 v0 = glm::vec3(m_v);
    glm::vec3 u = u0 * glm::cos(rad) -  v0 * glm::sin(rad);
    glm::vec3 v = u0 * glm::sin(rad) +  v0 * glm::cos(rad);
    m_u = glm::vec4(u, 1);
    m_v = glm::vec4(v, 1);
    updateViewMatrix();

}

void CamtransCamera::setClip(float nearPlane, float farPlane) {
    // @TODO: [CAMTRANS] Fill this in...
    m_near = nearPlane;
    m_far = farPlane;
    updateProjectionMatrix();


}
void CamtransCamera::updateProjectionMatrix(){
    updateScaleMatrix();
    updatePerspectiveMatrix();

}

void CamtransCamera::updatePerspectiveMatrix(){
    float c = -m_near/m_far;
    glm::mat4 token = glm::mat4(1,  0,  0,  0,
                                0,  1,  0,  0,
                                0,  0,  -1/(c+1),   c/(c+1),
                                0,  0,  -1, 0);
    m_perspectiveTransformation = glm::transpose(token);


}
void CamtransCamera::updateScaleMatrix(){
    float radH = m_thetaH;
    float radW = m_thetaW;

    float fir_tkn = cot(radW/2)/m_far;
    float sec_tkn = cot(radH/2)/m_far;
    float thd_tkn = 1/m_far;
    glm::mat4 token = glm::mat4(fir_tkn,    0,      0,      0,
                                0,       sec_tkn,   0,      0,
                                0,          0,    thd_tkn,  0,
                                0,          0,      0,      1);
    m_scaleMatrix = glm::transpose(token);

}
void CamtransCamera::updateViewMatrix(){
    std::cout<<"Call updateViewMatrix "<<std::endl;
    updateTranslationMatrix();
    updateRotationMatrix();

}
void CamtransCamera::updateRotationMatrix(){
    glm::mat4 token = glm::mat4(m_u.x, m_u.y, m_u.z, 0,
                                m_v.x, m_v.y, m_v.z, 0,
                                m_w.x, m_w.y, m_w.z, 0,
                                0,     0,     0,      1);
    m_rotationMatrix  = glm::transpose(token);

}
void CamtransCamera::updateTranslationMatrix(){
    std::cout<<"Call updateTranslationMatrix() "<<std::endl;
    glm::mat4 token = glm::mat4(1, 0, 0, -m_eye.x,
                                0, 1, 0, -m_eye.y,
                                0, 0, 1, -m_eye.z,
                                0, 0, 0, 1);
    m_translationMatrix = glm::transpose(token);

}



