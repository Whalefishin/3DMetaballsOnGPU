#include "camera.h"

using namespace cs40;

Camera::Camera() {

}

void Camera::right(float amt){
    m_lookAt.translate(amt, 0, 0);
    vec3 translation(amt,0,0);
    curr_position += translation;
}
void Camera::up(float amt){/* move camera up */
    m_lookAt.translate(0, amt, 0);
    vec3 translation(0,amt,0);
    curr_position += translation;
}
void Camera::forward(float amt){ /* move camera forward */
    m_lookAt.translate(0, 0, amt);
    vec3 translation(0,0,-amt);
    curr_position += translation;
}


/* rotate CCW about -z axis _of camera_
* angle is in degrees */
void Camera::roll(float angle){
    m_lookAt.rotate(angle, 0, 0, -1);
    currUp = m_lookAt.row(1);
}

/* rotate CCW about +x */
void Camera::pitch(float angle){
    m_lookAt.rotate(angle, 1, 0, 0);
    currUp = m_lookAt.row(1);
}

 /* CCW about +y */
void Camera::yaw(float angle){
    m_lookAt.rotate(angle, 0, 1, 0);
}

QMatrix4x4 Camera::lookAt(const vec3 &eye, const vec3 &at, const vec3 &up){

    vec3 zaxis = (eye - at) / (eye - at).length();
    vec3 xaxis = QVector3D::crossProduct(up ,zaxis) / QVector3D::crossProduct(up, zaxis).length();
    vec3 yaxis = QVector3D::crossProduct(zaxis, xaxis);

    QMatrix4x4 lookAt(
        xaxis.x(), xaxis.y(), xaxis.z(), -QVector3D::dotProduct(xaxis, eye),
        yaxis.x(), yaxis.y(), yaxis.z(), -QVector3D::dotProduct(yaxis, eye),
        zaxis.x(), zaxis.y(), zaxis.z(), -QVector3D::dotProduct(zaxis, eye),
        0, 0, 0, 1
    );

    m_lookAt = lookAt;
    return lookAt;
}
void reset(){}
